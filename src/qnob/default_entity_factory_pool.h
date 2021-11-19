#pragma once

#include <core/entity/entity_factory_pool.h>
#include <core/knob/knob.h>
#include <core/hotkey/hotkey.h>
#include <core/sound/sound.h>
#include <core/skin/horizontal_bar_skin.h>
#include <core/skin/numeric_skin.h>
#include <core/osd/osd.h>
#include <core/tray_icon/tray_icon.h>
#include <core/tray_event/tray_event.h>
#include <core/win10/win10.h>

class DefaultEntityFactoryPool : public EntityFactoryPool {
    Q_OBJECT
public:
    DefaultEntityFactoryPool(QObject* parent) : EntityFactoryPool(parent) {
        registerSimpleFactory<Hotkey>(lit("hotkey"));
        registerSimpleFactory<Knob>(lit("knob"));
        registerSimpleFactory<Sound>(lit("sound"));
        registerSimpleFactory<NumericSkin>(lit("numeric_skin"));
        registerSimpleFactory<HorizontalBarSkin>(lit("horizontal_bar_skin"));
        registerSimpleFactory<Osd>(lit("osd"));
        registerSimpleFactory<TrayIcon>(lit("tray_icon"));
        registerSimpleFactory<TrayEvent>(lit("tray_event"));
        registerSimpleFactory<Win10>(lit("win10"));
    }
};
