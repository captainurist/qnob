#pragma once

#include <cassert>

#include <QtCore/QVariant>
#include <QtCore/QMetaType>

#include "stl_fwd.h"

/* QVariantList/Map/Hash use Qt containers, which are not STL compatible in a lot of weird ways.
 * So we need our own containers. */

using VariantList = std::vector<QVariant>;
using VariantMap = std::map<QString, QVariant>;
using VariantHash = std::unordered_map<QString, QVariant>;


/* And we also need some magic to have compile-time metatype ids for our custom container classes. */

namespace MetaType {
    enum {
        VariantList = QMetaType::User - 1,
        VariantMap = QMetaType::User - 2,
        VariantHash = QMetaType::User - 3
    };
}

Q_DECLARE_BUILTIN_METATYPE("VariantList", MetaType::VariantList, VariantList)
Q_DECLARE_BUILTIN_METATYPE("VariantMap", MetaType::VariantMap, VariantMap)
Q_DECLARE_BUILTIN_METATYPE("VariantHash", MetaType::VariantHash, VariantHash)


/* Also now that we've decided to use STL containers we need to make `QVariant` use its internal storage for them. */

#define X_DECLARE_MOVABLE_CONTAINER(CONTAINER)                                                                         \
template<class... T>                                                                                                    \
class QTypeInfo<CONTAINER<T...>> {                                                                                      \
public:                                                                                                                 \
    enum {                                                                                                              \
        isPointer = false,                                                                                              \
        isIntegral = false,                                                                                             \
        isComplex = true,                                                                                               \
        isRelocatable = true,                                                                                           \
    };                                                                                                                  \
};

X_DECLARE_MOVABLE_CONTAINER(std::vector)
X_DECLARE_MOVABLE_CONTAINER(std::map)
X_DECLARE_MOVABLE_CONTAINER(std::set)
X_DECLARE_MOVABLE_CONTAINER(std::unordered_map)
X_DECLARE_MOVABLE_CONTAINER(std::unordered_set)
#undef X_DECLARE_MOVABLE_CONTAINER


// TODO: push upstream?
/**
 * Constructs a `QVariant` by moving a provided rvalue into its internal storage. Note that `QVariant` doesn't provide
 * this functionality - it always performs a copy.
 *
 * \param value                         An rvalue to construct a `QVariant` from.
 * \returns                             `QVariant` wrapping the provided value.
 */
template<class T>
QVariant variantFromRValue(T&& value) {
    static_assert(!std::is_lvalue_reference_v<T>, "Expected an rvalue reference.");

    QVariant result(QMetaType::fromType<T>());
    *static_cast<T*>(result.data()) = std::move(value);
    return result;
}

/**
 * This basically is a zerocopy read accessor for `QVariant`. Note that `QVariant` doesn't provide this functionality -
 * it always performs a copy.
 *
 * \param value                         Variant to get a reference for.
 * \returns                             Reference to the value stored inside a `QVariant`.
 */
template<class T>
const T& variantValueRef(const QVariant& value) {
    assert(value.typeId() == qMetaTypeId<T>());
    return *static_cast<const T*>(value.data());
}

template<class T>
T& variantValueRef(QVariant& value) {
    assert(value.typeId() == qMetaTypeId<T>());
    return *static_cast<T*>(value.data());
}

template<class T>
T&& variantValueRef(QVariant&& value) {
    assert(value.typeId() == qMetaTypeId<T>());
    return std::move(*static_cast<T*>(value.data()));
}
