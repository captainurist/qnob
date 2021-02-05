QT += widgets

CONFIG += c++latest
CONFIG += strict_c++

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QT_NO_CAST_FROM_ASCII
DEFINES += QT_NO_CAST_TO_ASCII
DEFINES += QT_NO_NARROWING_CONVERSIONS_IN_CONNECT

HEADERS += \
    src/config/config_exception.h \
    src/config/entity_config.h \
    src/config/full_config.h \
    src/core/entity.h \
    src/core/entity_creation_exception.h \
    src/core/entity_factory.h \
    src/core/entity_factory_pool.h \
    src/core/entity_pool.h \
    src/core/entity_pool_builder.h \
    src/core/entity_resolver.h \
    src/knob/knob.h \
    src/knob/volume_knob.h \
    src/platform/platform.h \
    src/platform/platform_shortcut_notifier.h \
    src/platform/platform_volume_control.h \
    src/platform/win/com.h \
    src/platform/win/com_ptr.h \
    src/platform/win/win_error.h \
    src/platform/win/win_platform.h \
    src/platform/win/win_shortcut_dispatcher.h \
    src/platform/win/win_volume_control.h \
    src/trigger/trigger.h \
    src/trigger/hotkey_trigger.h \
    src/trigger/hotkey_trigger_factory.h \
    src/utility/debug.h \


SOURCES += \
    src/config/full_config.cpp \
    src/core/entity_factory_pool.cpp \
    src/core/entity_pool.cpp \
    src/core/entity_pool_builder.cpp \
    src/knob/knob.cpp \
    src/knob/volume_knob.cpp \
    src/platform/win/com.cpp \
    src/platform/win/win_error.cpp \
    src/platform/win/win_platform.cpp \
    src/platform/win/win_shortcut_dispatcher.cpp \
    src/platform/win/win_volume_control.cpp \
    src/trigger/hotkey_trigger.cpp \
    src/trigger/hotkey_trigger_factory.cpp \
    src/utility/debug.cpp \
    src/utility/bound_meta_call.cpp \
    src/utility/meta_object_extension.cpp \
    src/utility/meta_method_invoker.cpp \
    src/main.cpp \


LIBS += \
    -lole32 \
    -luser32 \


INCLUDEPATH += \
    src \
    3rdparty/tomlplusplus/include \


UI_DIR    = src/ui
MOC_DIR   = bin/temp/moc
RCC_DIR   = bin/temp/rcc
TARGET    = qnob

# Full path of source code file in diagnostics
QMAKE_CXXFLAGS += /FC

CONFIG(debug, debug|release) {
    CONFIG           += console
    DESTDIR         = bin/debug
    OBJECTS_DIR     = bin/debug
}

CONFIG(release, debug|release) {
    CONFIG           -= console
    DESTDIR         = bin/release
    OBJECTS_DIR     = bin/release
}

