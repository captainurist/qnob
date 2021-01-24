#include <QtWidgets/QApplication>

#include <platform/platform.h>
#include <platform/platform_shortcut_notifier.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PlatformShortcutNotifier* notifier = qPlatform()->createShortcutNotifier(QKeySequence(QLatin1String("Shift+Pause")));

    QObject::connect(notifier, &PlatformShortcutNotifier::activated, [] { qDebug() << "!!!!1"; });

    return a.exec();
}
