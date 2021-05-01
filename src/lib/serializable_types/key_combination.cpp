#include "key_combination.h"

#include <QtCore/QString>
#include <QtGui/QKeySequence>

#include <lib/serialization/serialization_exceptions.h>

void serialize(const QKeyCombination& src, QString* dst) {
    if (src.key() == Qt::Key_unknown) {
        dst->clear();
        return;
    }

    *dst = QKeySequence(src).toString(QKeySequence::PortableText);
}

void deserialize(QStringView src, QKeyCombination* dst) {
    if (src.empty()) {
        *dst = QKeyCombination();
        return; /* Special handling for invalid key combinations so that they are serializable. */
    }

    QKeySequence keys = QKeySequence::fromString(src.toString(), QKeySequence::PortableText);
    if (keys.count() != 1 || keys[0].key() == Qt::Key_unknown)
        throwDeserializationException(src, typeid(QKeyCombination));
    *dst = keys[0];
}
