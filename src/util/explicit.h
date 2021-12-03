#pragma once

#include <type_traits>

// TODO docs that actually talk about how T is stored
/**
 * A reference wrapper that provides a functionality akin to `explicit` keyword, but for function parameters. Tagging
 * a parameter type as `Explicit` disables all implicit conversions and requires to perform an explicit conversion at
 * the call site.
 *
 * Example:
 * ```
 * void f(Explicit<int> a);
 *
 * f('a'); // Compilation error.
 * f(1l); // Compilation error.
 * f(1); // OK.
 * ```
 */
template<class T>
class Explicit {
public:
    template<class Other> requires std::is_same_v<std::remove_cvref_t<Other>, std::remove_cvref_t<T>>
    Explicit(Other&& value) :
        m_value(std::forward<Other>(value))
    {}

    T operator*() const {
        return m_value;
    }

private:
    T m_value;
};
