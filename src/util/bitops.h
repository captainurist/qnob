#pragma once

#include <cassert>

template<class T>
bool isPower2(T value) {
    return value > 0 && (value & (value - 1)) == 0;
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
