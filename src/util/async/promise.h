#pragma once

#include <cassert>
#include <type_traits>
#include <memory>

#include "future_state.h"

template<class T>
class Promise {
    static constexpr bool is_void_promise = std::is_same_v<T, void>;
    using State = detail::FutureState<std::conditional_t<is_void_promise, std::monostate, T>>;
public:
    Promise():
        m_state(std::make_shared<State>())
    {}

    Promise(const Promise&) = delete;
    Promise(Promise&&) = default;

    ~Promise() {
        assert(!m_state); /* We got a broken promise, and in this implementation that's a hard error. */
    }

    Future<T> future() const {
        assert(m_state.use_count() == 1); /* One future per promise please! */

        return Future<T>(m_state);
    }

    void set_value(T&& result) requires !is_void_promise{
        auto state = take_state();
        state->finish_with_value(std::move(state), std::move(result));
    }

    void set_value() requires is_void_promise {
        auto state = take_state();
        state->finish_with_value(std::move(state), std::monostate());
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
