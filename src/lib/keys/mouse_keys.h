#pragma once

#include <QtCore/Qt>

namespace Qt {
    constexpr Qt::Key Key_Mouse1 = static_cast<Qt::Key>(Qt::Key_unknown - 2);
    constexpr Qt::Key Key_Mouse2 = static_cast<Qt::Key>(Qt::Key_unknown - 3);
    constexpr Qt::Key Key_Mouse3 = static_cast<Qt::Key>(Qt::Key_unknown - 4);
    constexpr Qt::Key Key_WheelUp = static_cast<Qt::Key>(Qt::Key_unknown - 5);
    constexpr Qt::Key Key_WheelDown = static_cast<Qt::Key>(Qt::Key_unknown - 6);
    constexpr Qt::Key Key_WheelLeft = static_cast<Qt::Key>(Qt::Key_unknown - 7);
    constexpr Qt::Key Key_WheelRight = static_cast<Qt::Key>(Qt::Key_unknown - 8);
}

constexpr bool isMouseWheel(Qt::Key key) {
    switch (key) {
    case Qt::Key_WheelUp:
    case Qt::Key_WheelDown:
    case Qt::Key_WheelLeft:
    case Qt::Key_WheelRight:
        return true;
    default:
        return false;
    }
}
