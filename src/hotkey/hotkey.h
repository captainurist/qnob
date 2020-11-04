#pragma once

#include <QtCore/Qt>

struct Hotkey {
    Qt::KeyboardModifiers modifiers;
    Qt::Key key;

    friend qHash(const Hotkey& hotkey, uint seed = 0) {
        return qHash(hotkey.key, seed) + modifiers;
    }
};
