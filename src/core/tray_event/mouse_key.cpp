#include "mouse_key.h"

#include <lib/serialization/enum_serialization.h>

QB_DEFINE_ENUM_SERIALIZATION_FUNCTIONS(MouseKey, ({
    {Key_WheelUp, "mwheelup"},
    {Key_WheelDown, "mwheeldown"}
}))
