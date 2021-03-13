#pragma once

#include <cassert>
#include <map>
#include <vector>
#include <unordered_map>

#include <QtCore/QVariant>
#include <QtCore/QMetaType>

using VariantVector = std::vector<QVariant>;
using VariantMap = std::map<QString, QVariant>;
using VariantHash = std::unordered_map<QString, QVariant>;

namespace MetaType {
    enum {
        VariantVector = QMetaType::User - 1,
        VariantMap = QMetaType::User - 2,
        VariantHash = QMetaType::User - 3
    };
}

Q_DECLARE_BUILTIN_METATYPE("VariantVector", MetaType::VariantVector, VariantVector)
Q_DECLARE_BUILTIN_METATYPE("VariantMap", MetaType::VariantMap, VariantMap)
Q_DECLARE_BUILTIN_METATYPE("VariantHash", MetaType::VariantHash, VariantHash)

template<class T>
QVariant variantFromRValue(T&& value) {
    static_assert(!std::is_lvalue_reference_v<T>, "Expected an rvalue reference.");

    QVariant result(QMetaType::fromType<T>());
    *static_cast<T*>(result.data()) = std::move(value);
    return result;
}

template<class T>
const T& variantValueRef(const QVariant& value) {
    assert(value.typeId() == qMetaTypeId<T>());
    return *static_cast<const T*>(value.data());
}
