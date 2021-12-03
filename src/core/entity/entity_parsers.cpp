#include "entity_parsers.h"

#include <QtCore/QFileInfo>
#include <QtGui/QPixmap>
#include <QtGui/QKeySequence>

#include <util/exception/bad_cast_exception.h>

template<class T>
static void parseConfigValueWithVariantCast(const QVariant& from, T* to) {
    static_assert(qMetaTypeId<T>() != 0);

    if (!QMetaType::convert(from.metaType(), from.constData(), QMetaType::fromType<T>(), to))
        xthrow BadCastException(from.metaType(), QMetaType::fromType<T>());
}

void parseConfigValue(const EntityConfig&, const QVariant& from, QVariant* to, nullptr_t) {
    *to = from;
}

void parseConfigValue(const EntityConfig&, const QVariant& from, QString* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityConfig&, const QVariant& from, double* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityConfig&, const QVariant& from, qint64* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, bool* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityConfig&, const QVariant& from, VariantList* to, nullptr_t) {
    parseConfigValueWithVariantCast(from, to);
}

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QString* to, AsPath) {
    parseConfigValue(ctx, from, to, nullptr);

    if (!QFileInfo(*to).isAbsolute()) {
        bool endsWithSlash = ctx.basePath().endsWith(QLatin1Char('/')) || ctx.basePath().endsWith(QLatin1Char('\\'));
        *to = ctx.basePath() + (endsWithSlash ? QString() : lit("/")) + *to;
    }

    if (!QFileInfo::exists(*to))
        xthrow BadCastException(sformat(BadCastException::tr("'{}' is not a valid file path."), *to));
}

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QPixmap* to, nullptr_t) {
    QString path;
    parseConfigValue(ctx, from, &path, AsPath());

    *to = QPixmap(path);
    if (to->isNull())
        xthrow BadCastException(sformat(BadCastException::tr("'{}' is not a valid image file."), path));
}

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QPoint* to, nullptr_t) {
    std::vector<qint64> list;
    parseConfigValue(ctx, from, &list, nullptr);

    if (list.size() != 2)
        xthrow BadCastException(BadCastException::tr("2D point must have exactly 2 coordinates."));

    *to = QPoint(list[0], list[1]);
}

void parseConfigValue(const EntityConfig& ctx, const QVariant& from, QSize* to, nullptr_t) {
    std::vector<qint64> list;
    parseConfigValue(ctx, from, &list, nullptr);

    if (list.size() != 2)
        xthrow BadCastException(BadCastException::tr("2D size must have exactly 2 coordinates."));

    *to = QSize(list[0], list[1]);
}
