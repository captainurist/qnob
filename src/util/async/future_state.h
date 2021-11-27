#pragma once

#include <type_traits>
#include <variant>
#include <stop_token>

#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QWaitCondition>

#include "future_traits.h"
#include "future_result.h"
#include "future_exception.h"

namespace detail {

template<class T>
using maybe_monostate_t = std::conditional_t<std::is_same_v<T, void>, std::monostate, T>;

template<class ResultType, bool useStopToken, class Callback, class... Args>
ResultType invoke_future_callback(Callback&& callback, std::stop_token* token, Args&&... args) {
    if constexpr (useStopToken) {
        if constexpr (std::is_same_v<ResultType, std::monostate>) {
            std::invoke(std::forward<Callback>(callback), std::forward<Args>(args)..., std::move(*token));
            return std::monostate();
        } else {
            return std::invoke(std::forward<Callback>(callback), std::forward<Args>(args)..., std::move(*token));
        }
    } else {
        if constexpr (std::is_same_v<ResultType, std::monostate>) {
            std::invoke(std::forward<Callback>(callback), std::forward<Args>(args)...);
            return std::monostate();
        } else {
            return std::invoke(std::forward<Callback>(callback), std::forward<Args>(args)...);
        }
    }
}


class FutureStateBase {
    using callback_type = std::function<void(void*)>;
public:
    FutureStateBase() = default;
    FutureStateBase(const FutureStateBase&) = delete;
    FutureStateBase(FutureStateBase&&) = delete;

    enum ContinuationType {
        /** It's not yet clear what the user wants from us on the `Future<T>` side. */
        UnknownContinuation,

        /** User has attached a continuation. */
        CallbackContinuation,

        /** User is waiting on a wait condition. */
        WaitConditionContinuation,

        /** User has called `cancel`, or the future was abandoned (`Future<T>` destructor called). */
        NoContinuation
    };

    void cancel_async() {
        QMutexLocker locker(&m_mutex);

        /* We were called from a child future, thus we must have a continuation. */
        assert(continuation() == CallbackContinuation);

        /* Might have already been canceled. */
        if (m_cancelled)
            return;
        m_cancelled = true;

        /* Notify stop tokens & invoke stop callbacks. Doing this under lock is intended. */
        m_stopSource.request_stop();

        /* We might have a connection to QObject::destroyed, and it's no longer needed. */
        QObject::disconnect(m_connection);

        /* Propagate cancellation. */
        std::shared_ptr<FutureStateBase> parent = m_weakParent.lock();
        locker.unlock(); /* Please no deadlocks. */
        if (parent)
            parent->cancel_async();
    }

    void detach() {
        QMutexLocker locker(&m_mutex);

        assert(continuation() == UnknownContinuation);
        assert(!m_cancelled); /* UnknownContinuation => cannot be already canceled. */

        m_continuation.emplace<NoContinuation>();
    }

protected:
    template<class T>
    friend class FutureState;

    ContinuationType continuation() const {
        assert(!m_mutex.try_lock());

        return static_cast<ContinuationType>(m_continuation.index());
    }

    void init_parent(std::weak_ptr<FutureStateBase> parent) {
        assert(parent.lock());

        m_weakParent = std::move(parent);
    }

    void init_connection(QMetaObject::Connection&& connection) {
        assert(connection);

        m_connection = std::move(connection);
    }

protected:
    mutable QMutex m_mutex;
    std::variant<std::monostate, callback_type, QWaitCondition, std::monostate> m_continuation;

    /* The rest of the state is for cancellation support for all the different use cases. 
     * It is possible to type-erase everything here, but it's not really worth it at the moment. 
     * 
     * Cancellation flag is a separate field because we cannot touch the continuation in `cancel_async`. */

    bool m_cancelled = false;
    std::stop_source m_stopSource = std::stop_source(std::nostopstate); /* Don't allocate stop state unless it's needed. */
    std::weak_ptr<FutureStateBase> m_weakParent;
    QMetaObject::Connection m_connection;
};


template<class T>
class FutureState: public FutureStateBase {
    static_assert(!std::is_same_v<T, void>, "Void must be handled at the interface level, FutureState works with std::monostate instead.");
public:
    using self_ptr = std::shared_ptr<FutureState<T>>;

    enum State {
        Running,
        ReadyWithValue,
        ReadyWithError
    };

    void cancel(bool wait) {
        QMutexLocker locker(&m_mutex);

        assert(continuation() == UnknownContinuation);
        assert(!m_cancelled); /* UnknownContinuation => cannot be already canceled. */
        assert(!m_connection); /* UnknownContinuation => cannot have a connection. */

        m_cancelled = true;

        /* Notify stop tokens & invoke stop callbacks. Doing this under lock is intended. */
        m_stopSource.request_stop();

        /* Propagate cancellation. */
        std::shared_ptr<FutureStateBase> parent = m_weakParent.lock();
        locker.unlock(); /* Please no deadlocks. */
        if (parent)
            parent->cancel_async();

        /* Wait if we're not simply canceling & detaching. */
        if (wait) {
            locker.relock();
            wait_locked();
        }
    }

    T get() {
        QMutexLocker locker(&m_mutex);

        assert(continuation() == UnknownContinuation);
        assert(!m_cancelled); /* UnknownContinuation => cannot be already canceled. */

        wait_locked();

        if (state() == ReadyWithValue) {
            return std::get<ReadyWithValue>(std::move(m_data));
        } else {
            assert(state() == ReadyWithError);
            std::rethrow_exception(std::get<ReadyWithError>(std::move(m_data)));
        }
    }

    template<class Callback>
    std::shared_ptr<FutureState<future_result_t<Callback, T>>> then(self_ptr&& source, QObject* context, Callback&& callback) {
        assert(context);

        QMutexLocker locker(&m_mutex);

        assert(continuation() == UnknownContinuation);
        assert(!m_cancelled); /* UnknownContinuation => cannot be already canceled. */

        auto target = std::make_shared<FutureState<future_result_t<Callback, T>>>();
        target->init_parent(source);

        /* Connection is needed even if we're ready because context object might be destroyed between a call
         * to QMetaObject::invokeMethod and the event actually getting delivered. */
        target->init_connection(QObject::connect(context, &QObject::destroyed, [weakTarget = std::weak_ptr(target)] {
            auto target = weakTarget.lock();
            if (target)
                target->finish_with_context_exception(std::move(target));
        }));

        if (state() == Running) {
            m_continuation.emplace<CallbackContinuation>(
                [context, target, callback = std::forward<Callback>(callback)] (void* magic) mutable {
                    self_ptr& source = *static_cast<self_ptr*>(magic);

                    target->finish_with_callback_later(std::move(target), context, std::move(callback), FutureResult(std::move(source)));
                }
            );
        } else {
            assert(state() == ReadyWithError || state() == ReadyWithValue);

            target->finish_with_callback_later(std::shared_ptr(target), context, std::forward<Callback>(callback), FutureResult(std::move(source)));
        }

        return target;
    }

    void finish_with_value(self_ptr&& self, T&& value) {
        finish_with_value_or_error(std::move(self), &value, std::exception_ptr());
    }

    void finish_with_error(self_ptr&& self, std::exception_ptr error) {
        assert(error);

        finish_with_value_or_error(std::move(self), nullptr, error);
    }

    template<class Callback, class SourceResult>
    void finish_with_callback_later(self_ptr&& self, QObject* context, Callback&& callback, FutureResult<SourceResult> source) {
        QMutexLocker locker(&m_mutex);

        if (m_cancelled) {
            finish_cancelled_locked();
            return;
        }

        if (state() != Running) {
            assert(state() == ReadyWithError); /* Context was destroyed & we're already finished. */
            return;
        }

        /* We're not canceled, context is alive, we have the lock. Safe to call QMetaObject::invokeMethod!
         *
         * It is possible not to use Qt::QueuedConnection here if we're in the same thread as context, but
         * it's just not worth optimizing for. */

        QMetaObject::invokeMethod(
            context,
            [target = std::move(self), callback = std::forward<Callback>(callback), source = std::move(source)]() mutable {
                target->finish_with_callback(std::move(target), std::move(callback), std::move(source));
            },
            Qt::QueuedConnection
        );
    }

    template<class Callback, class SourceResult>
    void finish_with_callback(self_ptr&& self, Callback&& callback, FutureResult<SourceResult> source) {
        using traits = future_traits<Callback, SourceResult>;
        constexpr FutureArgType arg = traits::arg;
        constexpr bool useToken = traits::uses_token;
        static_assert(arg != ErrorFutureArg, "Unsupported continuation argument type.");

        QMutexLocker locker(&m_mutex);

        assert(state() == Running);

        if (m_cancelled) {
            finish_cancelled_locked();
            return;
        }

        /* We got here from the context's event handler => we no longer need to listen to QObject::destroyed. */
        assert(m_connection);
        QObject::disconnect(m_connection);

        /* Lazy-init the stop source. */
        std::stop_token token;
        if constexpr (useToken) {
            if (!m_stopSource.stop_possible())
                m_stopSource = std::stop_source();
            token = m_stopSource.get_token();
        }

        if constexpr (arg == ValueFutureArg || arg == VoidFutureArg) {
            if (source.has_error()) {
                finish_with_value_or_error_locked(std::move(self), nullptr, source.error());
            } else {
                locker.unlock(); /* Don't run user-supplied callbacks under lock. */

                try {
                    if constexpr (arg == ValueFutureArg) {
                        finish_with_value(std::move(self), invoke_future_callback<SourceResult, useToken>(std::forward<Callback>(callback), &token, source.value()));
                    } else {
                        finish_with_value(std::move(self), invoke_future_callback<SourceResult, useToken>(std::forward<Callback>(callback), &token));
                    }
                } catch (...) {
                    finish_with_error(std::move(self), std::current_exception()); 
                }
            }
        } else {
            locker.unlock(); /* Don't run user-supplied callbacks under lock. */

            try {
                finish_with_value(std::move(self), invoke_future_callback<SourceResult, useToken>(std::forward<Callback>(callback), &token, std::move(source)));
            } catch (...) {
                finish_with_error(std::move(self), std::current_exception());
            }
        }
    }

    void finish_with_context_exception(self_ptr&& self) {
        QMutexLocker locker(&m_mutex);

        assert(state() == Running);

        finish_with_value_or_error_locked(std::move(self), nullptr, std::make_exception_ptr(FutureContextException()));

        /* Cancel parents. */
        std::shared_ptr<FutureStateBase> parent = m_weakParent.lock();
        locker.unlock(); /* Please no deadlocks. */
        if (parent)
            parent->cancel_async();
    }

    State ready_state() const {
        State result = static_cast<State>(m_data.index());

        assert(result == ReadyWithError || result == ReadyWithValue);

        return result;
    }

    std::exception_ptr take_error() {
        assert(ready_state() == ReadyWithError);

        return std::get<ReadyWithError>(std::move(m_data));
    }

    T&& take_value() {
        assert(ready_state() == ReadyWithValue);

        return std::get<ReadyWithValue>(std::move(m_data));
    }

private:
    State state() const {
        assert(!m_mutex.try_lock());

        return static_cast<State>(m_data.index());
    }

    void finish_with_value_or_error(self_ptr&& self, T* value, std::exception_ptr error) {
        QMutexLocker locker(&m_mutex);
        finish_with_value_or_error_locked(std::move(self), value, error);
    }

    void finish_with_value_or_error_locked(self_ptr&& self, T* value, std::exception_ptr error) {
        assert(static_cast<bool>(value) ^ static_cast<bool>(error)); /* Only one must be set. */
        assert(!m_mutex.try_lock());
        assert(state() == Running);

        if (continuation() == NoContinuation)
            return; /* Abandoned, results not needed. */

        if (m_cancelled) {
            finish_cancelled_locked();
            return;
        }

        if (value) {
            m_data.emplace<ReadyWithValue>(std::move(*value)); /* Assume move doesn't throw as per c++ code guidelines. */
        } else {
            m_data.emplace<ReadyWithError>(std::move(error));
        }

        if (continuation() == CallbackContinuation) {
            std::get<CallbackContinuation>(m_continuation)(&self);
        } else if (continuation() == WaitConditionContinuation) {
            std::get<WaitConditionContinuation>(m_continuation).wakeAll();
        }
    }

    void finish_cancelled_locked() {
        assert(!m_mutex.try_lock());
        assert(state() == Running);
        assert(m_cancelled);

        m_data.emplace<ReadyWithError>(std::exception_ptr());
        if (continuation() == WaitConditionContinuation)
            std::get<WaitConditionContinuation>(m_continuation).wakeAll();
    }

    void wait_locked() {
        assert(!m_mutex.try_lock());
        assert(continuation() == UnknownContinuation);

        if (state() != Running)
            return;
        
        m_continuation.emplace<WaitConditionContinuation>();
        while (state() == Running)
            std::get<WaitConditionContinuation>(m_continuation).wait(&m_mutex);
    }

private:
    std::variant<std::monostate, T, std::exception_ptr> m_data;
};

}
