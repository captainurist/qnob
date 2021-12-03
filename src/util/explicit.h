#pragma once

#include <type_traits>

/**
 * Wrapper type that provides a functionality akin to `explicit` keyword, but for function parameters. Tagging
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
 *
 * void g(Explicit<const QByteArray&> s);
 *
 * g("1"); // Compilation error.
 * g(QByteArray("1")); // OK.
 * ```
 *
 * \tparam T                            The wrapped type. Can be a reference, in which case a reference will be stored
 *                                      in the wrapper.
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

    std::remove_reference_t<T>* operator->() {
        return &m_value;
    }

private:
    T m_value;
};
