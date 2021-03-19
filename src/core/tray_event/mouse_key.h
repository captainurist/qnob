#pragma once

#include <lib/serialization/serialization_fwd.h>

// TODO: move this somewhere and provide a better design.

enum MouseKey {
    Key_WheelUp,
    Key_WheelDown
};

QB_DECLARE_SERIALIZATION_FUNCTIONS(MouseKey)
