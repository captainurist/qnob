#include "win10.h"

#include <platform/platform.h>

#include <core/entity/entity_config.h>

Win10::~Win10() {
    if (m_systemOsdHidden)
        if(auto control = platform()->createStandardControl(NativeOsdControl, this))
            control->setMinimized(false);
}

void Win10::loadFromConfig(const EntityConfig& cfg) {
    if (cfg.requireOr<bool>(lit("hide_native_osd"), false)) {
        ensureNativeOsdHidden();
        // TODO: also bind to settings changes!
    }
}

void Win10::ensureNativeOsdHidden() {
    /* It's better to recreate the control on every call as the underlying native window might get destroyed. */
    if (auto control = platform()->createStandardControl(NativeOsdControl, this)) {
        m_systemOsdHidden = true;
        control->setMinimized(true);
    }
}
