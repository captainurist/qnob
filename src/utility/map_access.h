#pragma once

#include <type_traits>

template<class Map, class Key, class Value = typename Map::mapped_type>
Value value(const Map& map, const Key& key, const std::type_identity_t<Value>& def = Value()) {
    auto pos = map.find(key);
    return pos == map.end() ? def : pos->second;
}


template<class Map, class Key, class Value = typename Map::mapped_type>
const Value& value_ref(const Map& map, const Key& key, const std::type_identity_t<Value>& def = Value()) {
    auto pos = map.find(key);
    return pos == map.end() ? def : pos->second;
}
