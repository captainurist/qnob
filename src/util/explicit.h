#pragma once

#include <type_traits>

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
    template<class Other> requires std::is_same_v<Other, T>
    Explicit(const Other& value) :
        m_value(value)
    {}

    const T& operator*() const {
        return m_value;
    }

private:
    const T& m_value;
};
