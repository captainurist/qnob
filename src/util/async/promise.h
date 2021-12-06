#pragma once

#include <cassert>
#include <type_traits>
#include <memory>

#include "future_state.h"

/**
 * Callee-facing part of the promise/future interface.
 *
 * Like `std::future`, the value can be set only once. Unlike `std::future`, setting the value multiple times results
 * in undefined behavior. Not setting any value is also undefined behavior, thus we don't have a `broken_promise`
 * equivalent.
 *
 * There is no cancellation API on the promise side, and this is by design. If we were to allow cancellations here,
 * then we'd need an API on the future side to query whether the computation was canceled, and this unnecessarily
 * complicates everything. Use exceptions if you need to signal cancellation.
 */
template<class T>
class Promise {
    static constexpr bool is_void_promise = std::is_same_v<T, void>;
    using State = detail::FutureState<T>;
    using result_type = typename State::result_type;
public:
    Promise():
        m_state(std::make_shared<State>())
    {}

    Promise(Promise&&) = default;
    Promise(const Promise&) = delete;
    Promise& operator=(Promise&&) = default;
    Promise& operator=(const Promise&) = delete;

    ~Promise() {
        assert(!m_state); /* We got a broken promise, and in this implementation that's undefined behavior. */
    }

    Future<T> future() const {
        assert(m_state.use_count() == 1); /* One future per promise please! */

        return Future<T>(m_state);
    }

    void set_value(result_type&& result) requires !is_void_promise {
        auto state = take_state();
        state->finish_with_value(std::move(state), std::move(result));
    }

    void set_value() requires is_void_promise {
        auto state = take_state();
        state->finish_with_value(std::move(state), result_type());
    }

    void set_error(std::exception_ptr error) {
        auto state = take_state();
        state->finish_with_error(std::move(state), error);
    }

private:
    std::shared_ptr<State> take_state() {
        assert(m_state);

        return std::move(m_state);
    }

private:
    std::shared_ptr<State> m_state;
};
