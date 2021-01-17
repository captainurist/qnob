CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= debug_and_release debug_and_release_target 

QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    src/knob/knob.h \
    src/knob/volume_knob.h \
    src/platform/platform.h \
    src/platform/volume_control.h \
    src/platform/win/com.h \
    src/platform/win/com_ptr.h \
    src/platform/win/com_exception.h \
    src/platform/win/win_platform.h \
    src/platform/win/win_volume_control.h \
    src/trigger/trigger.h \
    src/trigger/hotkey_trigger.h \
    src/utility/debug.h \

SOURCES += \
    src/knob/volume_knob.cpp \
    src/platform/win/com.cpp \
    src/platform/win/com_exception.cpp \
    src/platform/win/win_platform.cpp \
    src/platform/win/win_volume_control.cpp \
    src/trigger/hotkey_trigger.cpp \
    src/utility/debug.cpp \
    src/main.cpp \

LIBS += \
    -lole32 \
    -luser32 \

INCLUDEPATH += src

UI_DIR    = src/ui
MOC_DIR   = bin/temp/moc
RCC_DIR   = bin/temp/rcc
TARGET    = qnob

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

