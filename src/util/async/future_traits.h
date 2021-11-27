#pragma once

#include <type_traits>

#include "future_result.h"

namespace detail {

enum FutureArgType {
    ValueFutureArg,
    ErrorFutureArg,
    ComboFutureArg,
    VoidFutureArg
};

template<class Callback, class T>
struct future_traits;

template<class Callback, class T> requires std::is_invocable_v<Callback, T&&>
struct future_traits<Callback, T> {
    using result_type = std::invoke_result_t<Callback, T&&>;

    static constexpr FutureArgType arg = ValueFutureArg;
    static constexpr bool uses_token = false;
};

template<class Callback, class T>
using future_result_t = typename future_traits<Callback, T>::result_type;

}







