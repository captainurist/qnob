#include <fstream>

#include <QtWidgets/QApplication>

#include <platform/platform.h>
#include <platform/platform_shortcut_notifier.h>

#include <utility/exception.h>

#include "config/flat_config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PlatformShortcutNotifier* notifier = qPlatform()->createShortcutNotifier(QKeySequence(QLatin1String("Shift+Pause")));

    QObject::connect(notifier, &PlatformShortcutNotifier::activated, [] { qDebug() << "!!!!1"; });

    FlatConfig congig = FlatConfig::fromTomlFile(QLatin1String("qnob.toml"));

    return a.exec();
}
