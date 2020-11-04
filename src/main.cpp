#include <QtWidgets/QApplication>

#include <com/com.h>

#include <knob/volume_knob.h>
#include <trigger/hotkey_trigger.h>

#include <QtGui/QKeySequence>

int main(int argc, char *argv[])
{
    Com com;

    QApplication a(argc, argv);

    VolumeKnob knob;
    knob.setValue(knob.value() + 100000);

    HotkeyTrigger trigger;

    return a.exec();
}
