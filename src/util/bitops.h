#pragma once

#include <cassert>
#include <bit>

template<class T>
bool isPower2(T value) {
    return std::has_single_bit(value);
}

template<class T, class A>
T alignUp(T value, A alignment) {
    assert(isPower2(alignment));

    return (value + alignment - 1) & ~(alignment - 1);
}

template<class T, class A>
T alignDown(T value, A alignment) {
    assert(isPower2(alignment));

    return value & ~(alignment - 1);
}
