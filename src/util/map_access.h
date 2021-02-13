#pragma once

#include <type_traits>

template<class Map, class Key, class Value = typename Map::mapped_type>
Value value_or(const Map& map, const Key& key, const std::type_identity_t<Value>& def = Value()) {
    auto pos = map.find(key);
    return pos == map.end() ? def : pos->second;
}

/**
 * Warning! If you're using a temporary as `def`, then this function might return a reference to this temporary. Know
 * how object lifetimes are managed in c++ and use at your own risk.
 *
 * \param map                           Map to get a value reference from.
 * \param key                           Key to use.
 * \param def                           Default value to return if the map doesn't contain the provided key.
 * \returns                             Reference to the value stored for the provided key, or `def`.
 */
template<class Map, class Key, class Value = typename Map::mapped_type>
const Value& value_ref(const Map& map, const Key& key, const std::type_identity_t<Value>& def) {
    auto pos = map.find(key);
    return pos == map.end() ? def : pos->second;
}

template<class Map, class Key, class Value = typename Map::mapped_type>
const Value* value_ptr(const Map& map, const Key& key) {
    auto pos = map.find(key);
    return pos == map.end() ? nullptr : &pos->second;
}
