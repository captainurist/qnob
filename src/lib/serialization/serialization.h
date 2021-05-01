#pragma once

#include <QtCore/QString>
#include <QtCore/QStringView>

#include "builtin_serialization.h"

namespace detail {

struct Serializer {
    template<class T>
    QString operator()(const T& value) const {
        QString result;
        serialize(value, &result);
        return result;
    }
};

template<class T>
struct Deserializer {
    T operator()(QStringView string) const {
        T result;
        deserialize(string, &result);
        return result;
    }
};

} // namespace detail

/**
 * Ranges-friendly serialization object. Can be used as `range | std::views::transform(serialized)`.
 */
constexpr detail::Serializer serialized;

/**
 * Ranges-friendly deserialization object. Can be used as `range | std::views::transform(deserialized<int>)`.
 */
template<class T>
constexpr detail::Deserializer<T> deserialized;
