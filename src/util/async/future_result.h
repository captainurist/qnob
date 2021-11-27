#pragma once

#include <cassert>
#include <exception>
#include <type_traits>

namespace detail {
template<class T>
class FutureState;
}

// TODO: interface can be improved. clear on move!

template<class T>
class FutureResult {
    using State = detail::FutureState<T>;
public:
    FutureResult() = default;
    FutureResult(const FutureResult&) = delete;
    FutureResult(FutureResult&&) = default;

    FutureResult(std::shared_ptr<detail::FutureState<T>>&& state) :
        m_state(std::move(state))
    {
        assert(m_state->ready_state() != State::Running);
    }

    bool has_value() const {
        return m_state->ready_state() == State::ReadyWithValue;
    }

    T&& value() const requires !std::is_same_v<T, void> {
        assert(has_value());

        return m_state->take_value();
    }

    bool has_error() const {
        return m_state->ready_state() == State::ReadyWithError;
    }

    std::exception_ptr error() const {
        assert(has_error());

        return m_state->take_error();
    }

private:
    std::shared_ptr<detail::FutureState<T>> m_state;
};

