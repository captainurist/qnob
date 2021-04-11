#include "builtin_serialization.h"

#include <QtCore/QString>

void serialize(const QString& src, QString* dst) {
    *dst = src;
}

void deserialize(QStringView src, QString* dst) {
    *dst = src.toString();
}

