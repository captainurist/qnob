#pragma once

#include <type_traits>

class QString;

template<class T>
struct is_serializable : std::false_type {};

template<class T> requires requires (T a, QString* b) { serialize(a, b); }
struct is_serializable<T> : std::true_type {};

template<class T>
inline constexpr bool is_serializable_v = is_serializable<T>::value;

template<class T>
struct is_deserializable : std::false_type {};

template<class T> requires requires (QString a, T* b) { deserialize(a, b); }
struct is_deserializable<T> : std::true_type {};

template<class T>
inline constexpr bool is_deserializable_v = is_deserializable<T>::value;
