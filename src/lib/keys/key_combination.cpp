#include "key_combination.h"

#include <cassert>

#include <QtCore/QString>
#include <QtGui/QKeySequence>

#include <lib/serialization/serialization_exceptions.h>
#include <lib/serialization/enum_serialization.h>

#include "mouse_keys.h"

namespace detail {
    X_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(Qt::Key, Qt::CaseInsensitive, {
        { Qt::Key_Mouse1, "Mouse1" },
        { Qt::Key_Mouse2, "Mouse2" },
        { Qt::Key_Mouse3, "Mouse3" },
        { Qt::Key_WheelUp, "MouseWheelUp" },
        { Qt::Key_WheelUp, "MWheelUp" },
        { Qt::Key_WheelDown, "MouseWheelDown" },
        { Qt::Key_WheelDown, "MWheelDown" }
    })
}

void serialize(const QKeyCombination& src, QString* dst) {
    if (src.key() == Qt::Key_unknown) {
        dst->clear();
        return;
    }

    QString mouseKeyString;
    bool isMouseKey = detail::try_serialize(src.key(), &mouseKeyString);
    QKeyCombination tmp = src;
    if (isMouseKey)
        tmp = QKeyCombination(tmp.keyboardModifiers(), Qt::Key_X);

    *dst = QKeySequence(tmp).toString(QKeySequence::PortableText);

    if (isMouseKey) {
        assert(!dst->isEmpty() && dst->back() == QLatin1Char('X'));
        dst->chop(1);
        dst->append(mouseKeyString);
    }
}

void deserialize(QStringView src, QKeyCombination* dst) {
    if (src.empty()) {
        *dst = QKeyCombination();
        return; /* Special handling for invalid key combinations so that they are serializable. */
    }

    Qt::Key mouseKey = Qt::Key_unknown;
    int keyIndex = src.lastIndexOf(QLatin1Char('+')) + 1; /* Conveniently equals zero if no plus found. */
    bool isMouseKey = detail::try_deserialize(src.mid(keyIndex), &mouseKey);

    QString tmp = src.toString();
    if (isMouseKey) {
        tmp.resize(keyIndex);
        tmp.append(QLatin1Char('X'));
    }

    QKeySequence keys = QKeySequence::fromString(tmp, QKeySequence::PortableText);
    if (keys.count() != 1 || keys[0].key() == Qt::Key_unknown)
        throwDeserializationException(src, typeid(QKeyCombination));

    if (isMouseKey) {
        *dst = QKeyCombination(keys[0].keyboardModifiers(), mouseKey);
    } else {
        *dst = keys[0];
    }
}

void serialize(const Qt::Key& src, QString* dst) {
    serialize(QKeyCombination(src), dst);
}

void deserialize(QStringView src, Qt::Key* dst) {
    QKeyCombination key;
    deserialize(src, &key);

    if (key.keyboardModifiers() != Qt::NoModifier)
        throwDeserializationException(src, typeid(Qt::Key));

    *dst = key.key();
}
