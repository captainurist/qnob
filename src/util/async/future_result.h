#pragma once

#include <cassert>
#include <exception>
#include <type_traits>

namespace detail {
template<class T>
class FutureState;
}

template<class T>
class FutureResult {
    static constexpr bool is_void_future = std::is_same_v<T, void>;
    using State = detail::FutureState<T>;
    using result_type = typename State::result_type;
public:
    FutureResult() = default;
    FutureResult(FutureResult&&) = default;
    FutureResult(const FutureResult&) = delete;
    FutureResult& operator=(FutureResult&&) = default;
    FutureResult& operator=(const FutureResult&) = delete;

    FutureResult(std::shared_ptr<State>&& state) :
        m_state(std::move(state))
    {
        assert(m_state->ready_state() != State::Running);
    }

    T get() {
        if (has_error()) {
            std::rethrow_exception(error());
        } else {
            return value();
        }
    }

    bool has_value() const {
        return m_state->ready_state() == State::ReadyWithValue;
    }

    std::conditional_t<is_void_future, void, result_type&&> value() const {
        assert(has_value());

        if constexpr (is_void_future) {
            m_state->take_value();
        } else {
            return m_state->take_value();
        }
    }

    bool has_error() const {
        return m_state->ready_state() == State::ReadyWithError;
    }

    std::exception_ptr error() const {
        assert(has_error());

        return m_state->take_error();
    }

private:
    std::shared_ptr<State> m_state;
};

