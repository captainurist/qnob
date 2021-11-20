#pragma once

#include <utility>
#include <functional> /* For std::hash. */

#include <QtCore/QHashFunctions>

/* It's 2021 and we STILL DON'T HAVE std::hash<std::pair> in STL, how retarded is that? */

template<class A, class B>
struct std::hash<std::pair<A, B>> {
    size_t operator()(const std::pair<A, B>& value) const {
        return qHash(value);
    }
};
