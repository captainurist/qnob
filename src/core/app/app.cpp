#include "app.h"

#include <QtCore/QCoreApplication>

#include <util/debug.h>
#include <platform/platform.h>

App::App(const QString& id) :
    Entity(id)
{}

void App::exit() {
    QCoreApplication::quit();
}

void App::debugAction() {
    platform()->execute(WinUpdateCurrentToolTip);

    xDebug("Debug action");
}
