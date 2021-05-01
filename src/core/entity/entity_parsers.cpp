#include "entity_parsers.h"

#include <QtCore/QFileInfo>
#include <QtGui/QPixmap>
#include <QtGui/QKeySequence>

#include <util/bad_cast_exception.h>

template<class T>
static void parseConfigValueWithVariantCast(const QVariant& from, T* to) {
    static_assert(qMetaTypeId<T>() != 0);

    if (!QMetaType::convert(from.metaType(), from.constData(), QMetaType::fromType<T>(), to))
        xthrow BadCastException(from.metaType(), QMetaType::fromType<T>());
}

void parseConfigValue(const EntityCreationContext*, const QVariant& from, QVariant* to, nullptr_t) {
    *to = from;
}

void parseConfigValue(const EntityCreationContext*, const QVariant& from, QString* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityCreationContext*, const QVariant& from, double* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityCreationContext*, const QVariant& from, qint64* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityCreationContext*, const QVariant& from, VariantVector* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityCreationContext* ctx, const QVariant& from, QString* to, AsPath) {
    parseConfigValue(ctx, from, to, nullptr);

    *to = ctx->resolver()->resolvePath(*to);
    if (!QFileInfo::exists(*to))
        xthrow BadCastException(BadCastException::tr("'%1' is not a valid file path.").arg(*to));
}

void parseConfigValue(const EntityCreationContext* ctx, const QVariant& from, QPixmap* to, nullptr_t) {
    QString path;
    parseConfigValue(ctx, from, &path, AsPath());

    *to = QPixmap(path);
    if (to->isNull())
        xthrow BadCastException(BadCastException::tr("'%1' is not a valid image file.").arg(path));
}

void parseConfigValue(const EntityCreationContext* ctx, const QVariant& from, QPoint* to, nullptr_t) {
    std::vector<qint64> list;
    parseConfigValue(ctx, from, &list, nullptr);

    if (list.size() != 2)
        xthrow BadCastException(BadCastException::tr("2D point must contain exactly 2 coordinates."));

    *to = QPoint(list[0], list[1]);
}

void parseConfigValue(const EntityCreationContext* ctx, const QVariant& from, QSize* to, nullptr_t) {
    std::vector<qint64> list;
    parseConfigValue(ctx, from, &list, nullptr);

    if (list.size() != 2)
        xthrow BadCastException(BadCastException::tr("2D size must contain exactly 2 coordinates."));

    *to = QSize(list[0], list[1]);
}
