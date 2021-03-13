#include "entity_parsers.h"

#include <QtCore/QFileInfo>
#include <QtGui/QPixmap>
#include <QtGui/QKeySequence>

#include <util/bad_cast_exception.h>

template<class T>
static void parseConfigValueWithVariantCast(const QVariant& from, T* to) {
    static_assert(qMetaTypeId<T>() != 0);

    if (!QMetaType::convert(from.metaType(), from.constData(), QMetaType::fromType<T>(), to))
        qthrow BadCastException(from.metaType(), QMetaType::fromType<T>());
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
        qthrow BadCastException(BadCastException::tr("'%1' is not a valid file path.").arg(*to));
}

void parseConfigValue(const EntityCreationContext* ctx, const QVariant& from, QPixmap* to, nullptr_t) {
    QString path;
    parseConfigValue(ctx, from, &path, AsPath());

    *to = QPixmap(path);
    if (to->isNull())
        qthrow BadCastException(BadCastException::tr("'%1' is not a valid image file.").arg(path));
}

void parseConfigValue(const EntityCreationContext* ctx, const QVariant& from, QKeySequence* to, nullptr_t) {
    QString sequenceString;
    parseConfigValue(ctx, from, &sequenceString, nullptr);

    *to = QKeySequence::fromString(sequenceString);
    if (to->isEmpty() || (*to)[0].key() == Qt::Key_unknown)
        qthrow BadCastException(BadCastException::tr("'%1' is not a valid key sequence.").arg(sequenceString));
}
