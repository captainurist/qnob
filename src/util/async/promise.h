#pragma once

#include <cassert>
#include <type_traits>
#include <memory>

#include "future_state.h"

/**
 * Callee-facing class.
 *
 * Note that there is no cancellation API here, and this is by design. If we were to allow cancellations on the promise
 * side, then we'd need an API to query whether the computation was canceled on the future side, and this just
 * unnecessarily complicates everything.
 *
 * Use exceptions if you need to signal cancellation.
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
        assert(!m_state); /* We got a broken promise, and in this implementation that's a hard error. */
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
