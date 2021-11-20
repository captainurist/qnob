#pragma once

#include <type_traits>

#include <QtCore/QVariant>

#include <lib/serialization/serialization_concepts.h>
#include <util/exception/bad_cast_exception.h>
#include <util/variant.h>
#include <util/format.h>

#include "entity_config.h"
#include "entity_creation_exception.h"
#include "entity.h"
#include "entity_pool.h"

class QPixmap;

struct AsPath {};

/**
 * Main extension point for all config parsing. Overloads are to be added when needed.
 *
 * \param ctx                           Creation context, provides necessary callbacks.
 * \param from                          Value to parse.
 * \param to                            Value to write result to.
 * \param tag                           Validation tag, `nullptr_t` means no validation.
 */
template<class T, class ValidationTag>
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, T* to, ValidationTag tag) {
    static_assert(false, "Cannot parse type T.");
}

/* Basic parsers. */

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QVariant* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QString* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, double* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, qint64* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, bool* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, VariantVector* to, nullptr_t);

/* The only parser with validation so far - validates paths. */

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QString* to, AsPath);

/* Some custom parsers. */

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QPixmap* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QPoint* to, nullptr_t);
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QSize* to, nullptr_t);

/* Parser for entities. */

template<class DerivedEntity>
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, DerivedEntity** to, nullptr_t)
    requires std::is_base_of_v<Entity, DerivedEntity>
{
    QString id;
    parseConfigValue(ctx, from, &id, nullptr);

    Entity* entity = ctx.entityPool()->entity(id);
    if(!entity)
        xthrow EntityCreationException(ctx.id(), sformat(EntityCreationException::tr("Entity '{}' doesn't exist."), id));

    if constexpr (std::is_same_v<DerivedEntity, Entity>) {
        *to = entity;
    } else {
        *to = dynamic_cast<DerivedEntity*>(entity);
        if (!*to)
            xthrow BadCastException(typeid(*entity), typeid(DerivedEntity));
    }
}

/* Parser for deserializable objects. */

template<class Deserializable> requires deserializable<Deserializable>
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, Deserializable* to, nullptr_t) {
    QString string;
    parseConfigValue(ctx, from, &string, nullptr);
    deserialize(string, to);
}

/* Parser for lists. */
template<class List, class ValidationTag>
void parseConfigValue(const EntityConfig& ctx, const QVariant& from, List* to, ValidationTag tag)
    requires requires (List a) { a.emplace_back(); }
{
    VariantVector variantList;
    parseConfigValue(ctx, from, &variantList, nullptr);

    to->clear();
    for (const QVariant& variantElement : variantList)
        parseConfigValue(ctx, variantElement, &to->emplace_back(), tag);
}
