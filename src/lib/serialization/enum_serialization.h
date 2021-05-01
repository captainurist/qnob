#pragma once

#include <unordered_map>
#include <initializer_list>
#include <utility>
#include <concepts>

#include <QtCore/QString>
#include <QtCore/QStringView>
#include <QtCore/QHashFunctions>
#include <QtCore/QHash>

#include <util/string_pool.h>

#include "serialization_exceptions.h"

template<class T>
class EnumSerializer {
public:
    /**
     * Creates an enum serializer.
     *
     * Some notes on the mapping:
     * - Each enum value might map into several different strings. When serializing, the first string will be used.
     * - Each string value must map to exactly one enum value.
     * This basically provides support for enum aliases.
     */
    EnumSerializer(std::initializer_list<std::pair<T, const char*>> pairs) {
        for (const auto [value, string] : pairs) {
            QStringView storedString = m_pool.insert(QString::fromUtf8(string));

            if (!m_stringByEnum.contains(value))
                m_stringByEnum.emplace(value, storedString);

            assert(!m_enumByString.contains(storedString));
            m_enumByString.emplace(storedString, value);
        }
    }

    EnumSerializer() {}

    void deserialize(QStringView string, T* target) const {
        auto pos = m_enumByString.find(string);
        if (pos == m_enumByString.end())
            throwDeserializationException(string, typeid(T));
        *target = pos->second;
    }

    void serialize(T value, QString* target) const {
        auto pos = m_stringByEnum.find(value);
        if (pos == m_stringByEnum.end())
            throwSerializationException(typeid(value));
        *target += pos->second;
    }

private:
    StringPool m_pool;
    std::unordered_map<T, QStringView> m_stringByEnum;
    std::unordered_map<QStringView, T> m_enumByString;
};


#define QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(TYPE, INITIALIZER)                                                       \
Q_GLOBAL_STATIC_WITH_ARGS(EnumSerializer<TYPE>, g_initializer_ ## __LINE__, INITIALIZER)                                \
                                                                                                                        \
void serialize(const TYPE& value, QString* target) {                                                                    \
    g_initializer_ ## __LINE__->serialize(value, target);                                                               \
}                                                                                                                       \
                                                                                                                        \
void deserialize(QStringView string, TYPE* target) {                                                                    \
    g_initializer_ ## __LINE__->deserialize(string, target);                                                            \
}
