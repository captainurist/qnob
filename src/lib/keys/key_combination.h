#pragma once

#include <QtCore/Qt>

#include <lib/serialization/serialization_fwd.h>

namespace Qt {
    constexpr Qt::Key Key_Mouse1 = static_cast<Qt::Key>(Qt::Key_unknown - 2);
    constexpr Qt::Key Key_Mouse2 = static_cast<Qt::Key>(Qt::Key_unknown - 3);
    constexpr Qt::Key Key_Mouse3 = static_cast<Qt::Key>(Qt::Key_unknown - 4);
    constexpr Qt::Key Key_WheelUp = static_cast<Qt::Key>(Qt::Key_unknown - 5);
    constexpr Qt::Key Key_WheelDown = static_cast<Qt::Key>(Qt::Key_unknown - 6);
}

QB_DECLARE_SERIALIZATION_FUNCTIONS(QKeyCombination)
