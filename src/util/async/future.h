#pragma once

#include <cassert>
#include <memory>
#include <type_traits>

#include <QtCore/QFuture>

#include "future_state.h"
#include "future_traits.h"

template<class T>
class Promise;


/**
 * A future class akin to `std::future`, but better.
 *
 * A `QFuture` might have been a nice alternative, but unfortunately it tries to be many things at once and fails
 * at most of them. See for yourself:
 * - There is a `then` overload taking `QObject*`, but it doesn't work as you might expect it to. Unlike signal-slot
 *   connections, this one just crashes if the object is destroyed before the continuation is called.
 * - `QFuture` has a hard time figuring out whether it's `std::future` or `std::shared_future`, and tries to be both.
 * - Because `QFuture` is shared by default, when several futures are chained with `then`, there is no way to cancel
 *   the whole chain. Calling `cancel` on the resulting future just cancels the last action in the chain.
 * - Even then, it's not actually canceled - if the action was attached with sync execution policy, it's still invoked!
 * - And have you tried reading through all the fields in `QFutureInterfaceBase`? It's a mess.
 *
 * `std::experimental::future` is also questionable. Or rather, all futures that provide support for continuations are
 * questionable, because then we're mixing several concepts into one:
 * 1. Future as an event, a way to notify the caller that some action has completed.
 * 2. Future as a descriptor to an execution context that the user can use to run a continuation in.
 *
 * The problem here is that not all futures even have the execution context - think about ready future. This is why
 * implementors have to jump through hoops as Qt does with `QtFuture::Launch::Sync`, which starts the continuation in
 * the remote execution context if there is one, or right inside the call to `then` if there is none. This is OK when
 * STL pulls this trick with `std::stop_callback`, but it's totally totally not OK in case of long-running computations.
 * See for yourself:
 * 1. The want to run a chain of continuations in the same execution context is absolutely normal.
 * 2. The future API contains an inherent race that actually makes #1 impossible no matter how hard you try.
 *
 * The race is that the remote computation can complete before we've attached a continuation. It's possible to fix this
 * by locking on the promise end until it becomes clear what the caller wants to do with the future, but this breaks
 * in the case when the execution context simply doesn't exist (e.g. when the function returning a future calls
 * `promise::set_value` right away).
 *
 * So while we're waiting for executors that will hopefully land before I end up in a retirement home, here is a sane
 * future class that's actually a pleasure to use. What we have here:
 * - An interface similar to `std::future`. Yes, everything's type-erased. And yes, this is where `snake_case` naming
 *   is coming from.
 * - Built-in cancellation support that actually works.
 * - Continuations via `then` that require explicitly supplied execution context.
 * - `QObject`s as execution contexts, which in fact ends up working like signal-slot connections. That is, if the
 *   `QObject` is destroyed, the continuation is not invoked, and the future is made ready with an exception.
 * - Seamless support for coroutines.
 *
 * As you see, chaining several continuations to be run in the same execution context is not supported, and this is a
 * direction for future work. A separate class is needed for that, something like `Computation<T>`, that could then be
 * used like this:
 * \code
 * // async returns a Computation<T>
 * Future<T> = async(func1).then(func2).then(func3).start();
 * \endcode
 */
template<class T>
class Future {
    static constexpr bool is_void_future = std::is_same_v<T, void>;
    using State = detail::FutureState<T>;
    using result_type = typename State::result_type;
public:
    /**
     * Constructs an invalid future.
     */
    Future() = default;
    Future(Future&&) = default;
    Future(const Future&) = delete;
    Future& operator=(Future&&) = default;
    Future& operator=(const Future&) = delete;

    Future(std::shared_ptr<State> state) :
        m_state(std::move(state))
    {
        assert(valid()); /* Data must be valid. */
    }

    /**
     * Destructor.
     *
     * If this future is valid, destructor will call `detach`. It's up to the user to wait for the remote computation
     * to complete.
     */
    ~Future() {
        if (valid())
            detach();
    }

    /**
     * Adapts provided `QFuture`. Adapting an invalid `QFuture` results in an invalid future.
     *
     * \param future                    Future to adapt.
     */
    Future(QFuture<T>&& future);

    /**
     * \returns                         Whether this future is valid.
     */
    bool valid() const {
        return m_state != nullptr;
    }

    /**
     * Cancels all computations associated with this future and waits for their completion.
     *
     * This future must be valid, and will become invalid after this call.
     *
     * If this future is a part of a chain created with `then` calls, then all computations in the chain are canceled.
     * If this future has already finished, discards the results stored in the shared state.
     */
    void cancel() {
        take_state()->cancel(true);
    }

    /**
     * Detaches this future from the shared state.
     *
     * This future must be valid, and will become invalid after this call.
     *
     * It's up to the user to wait for the remote computation to complete (e.g. by calling `QThreadPool::waitForDone`).
     */
    void detach() {
        take_state()->detach();
    }

    /**
     * Detaches this future from the shared state, and cancels the remote computation.
     *
     * This future must be valid, and will become invalid after this call.
     *
     * It's up to the user to wait for the remote computation to complete (e.g. by calling `QThreadPool::waitForDone`).
     */
    void detach_and_cancel() {
        take_state()->cancel(false);
    }

    /**
     * Waits for this future to become ready, and either returns the computation result or throws an exception.
     *
     * This future must be valid, and will become invalid after this call.
     *
     * \returns                         Computation result.
     * \throws                          If the computation ended with an error.
     */
    T get() {
        if constexpr (is_void_future) {
            take_state()->get();
        } else {
            return take_state()->get();
        }
    }

    /**
     * Coroutine support.
     */
    //void operator co_await(); // TODO

    /**
     * Attaches a continuation to this future.
     *
     * This future must be valid, and will become invalid after this call.
     *
     * Provided continuation will be executed in the thread associated with `context`. That thread must have
     * a running event loop, otherwise the continuation will never get called.
     *
     * If `context` is destroyed before this future is ready, this future becomes ready with `FutureContextException`
     * error.
     *
     * Continuation can accept:
     * - `T`. The continuation will be called when this future is ready and has a value. If this future becomes ready
     *   with an exception, the continuation will be skipped.
     * - Nothing. Same as above, but the value will be discarded.
     * - `ReadyFuture<T>`. This can be handy if you want to do exception handling in the continuation.
     * - `ReadyFuture<void>`. Same as above, but the value will be discarded.
     * - `std::stop_token` as the 2nd argument. This is a way to communicate cancellations to the running operation,
     *   or to get cancellation notifications via `std::stop_callback`.
     *
     * \param context                   `QObject`-derived target. Must not be null.
     * \param f                         Continuation functor.
     */
    template<class QObjectDerived, class Callback>
    Future<detail::future_result_t<Callback, T>> then(QObjectDerived* context, Callback&& callback) {
        auto state = take_state();
        return state->then(std::move(state), context, std::forward<Callback>(callback));
    }

    /**
     * Attaches a continuation to this future that is executed only if this future has ended with an error.
     *
     * This future must be valid, and will become invalid after this call.
     *
     * Continuation can accept:
     * - `std::exception_ptr`. This is the exception that was thrown. It is OK to rethrow it (or some other exception)
     *   from the continuation.
     * - Nothing. In this case the exception is discarded.
     *
     * \param f                         Continuation functor.
     */
    template<class Function>
    Future<T> catch_error(Function&& f);

private:
    std::shared_ptr<State> take_state() {
        assert(valid());

        return std::move(m_state);
    }

private:
    std::shared_ptr<State> m_state;
};
