#include "color.h"

#include <lib/serialization/serialization_exceptions.h>

void serialize(const QColor& src, QString* dst) {
    if (!src.isValid()) {
        dst->clear();
        return;
    }

    *dst = src.name(QColor::HexArgb);
}

void deserialize(QStringView src, QColor* dst) {
    if (src.empty()) {
        *dst = QColor();
        return; /* Special handling for invalid colors so that they are serializable. */
    }

    dst->setNamedColor(src);
    if (!dst->isValid())
        throwDeserializationException(src, typeid(QColor));
}
