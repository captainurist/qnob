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
    EnumSerializer() {}

    /**
     * Creates an enum serializer.
     *
     * Some notes on the mapping:
     * - Each enum value might map into several different strings. When serializing, the first string will be used.
     * - Each string value must map to exactly one enum value.
     * This basically provides support for enum aliases.
     */
    EnumSerializer(Qt::CaseSensitivity caseSensitivity, std::initializer_list<std::pair<T, const char*>> pairs) :
        m_caseSensitivity(caseSensitivity)
    {
        for (const auto [value, name] : pairs) {
            QString string = QString::fromUtf8(name);

            if (!m_stringByEnum.contains(value))
                m_stringByEnum.emplace(value, m_pool.insert(string));

            if (m_caseSensitivity == Qt::CaseInsensitive)
                string = string.toLower();

            assert(!m_enumByString.contains(string));
            m_enumByString.emplace(m_pool.insert(string), value);
        }
    }

    void deserialize(QStringView string, T* target) const {
        if (m_caseSensitivity == Qt::CaseInsensitive) {
            deserializeInternal(string.toString().toLower(), target);
        } else {
            deserializeInternal(string, target);
        }
    }

    void serialize(T value, QString* target) const {
        auto pos = m_stringByEnum.find(value);
        if (pos == m_stringByEnum.end())
            throwSerializationException(typeid(value));
        *target += pos->second;
    }

private:
    void deserializeInternal(QStringView string, T* target) const {
        auto pos = m_enumByString.find(string);
        if (pos == m_enumByString.end())
            throwDeserializationException(string, typeid(T));
        *target = pos->second;
    }


private:
    Qt::CaseSensitivity m_caseSensitivity;
    StringPool m_pool;
    std::unordered_map<T, QStringView> m_stringByEnum;
    std::unordered_map<QStringView, T> m_enumByString;
};


/**
 * This macro generates enum serialization functions.
 *
 * Usage example:
 * ```
 * QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(Qt::Key, Qt::CaseInsensitive, {
 *     { Qt::Key_ScrollLock, "Scroll Lock" },
 *     { Qt::Key_ScrollLock, "ScrollLock" },    // Alternative string, used only for deserialization.
 *     { Qt::Key_ScrollLock, "SCRLK" },         // Can have several alternative strings.
 *     // ...
 * })
 * ```
 */
#define QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(TYPE, CASE_SENSITIVITY, ... /* INITIALIZER */)                           \
Q_GLOBAL_STATIC_WITH_ARGS(EnumSerializer<TYPE>, g_initializer_ ## __LINE__, (CASE_SENSITIVITY, __VA_ARGS__))            \
                                                                                                                        \
void serialize(const TYPE& value, QString* target) {                                                                    \
    g_initializer_ ## __LINE__->serialize(value, target);                                                               \
}                                                                                                                       \
                                                                                                                        \
void deserialize(QStringView string, TYPE* target) {                                                                    \
    g_initializer_ ## __LINE__->deserialize(string, target);                                                            \
}
