#include "app.h"

#include <QtCore/QCoreApplication>

#include <util/debug.h>
#include <platform/platform.h>

void App::exit() {
    QCoreApplication::quit();
}

void App::debugAction() {
    platform()->execute(WinUpdateCurrentToolTip);

    xDebug("Debug action");
}
