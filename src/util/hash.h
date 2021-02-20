#pragma once

#include <utility>
#include <functional> /* For std::hash. */

#include <QtCore/QHashFunctions>

namespace std {

    /* It's 2021 and we STILL DON'T HAVE std::hash<std::pair> in STL, how retarded is that? */

    template<class A, class B>
    struct hash<pair<A, B>> {
        size_t operator()(const pair<A, B>& value) const {
            return qHash(value);
        }
    };

}
