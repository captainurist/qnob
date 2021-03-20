#include "app.h"

#include <QtCore/QCoreApplication>

#include <platform/platform.h>

#include <QtCore/QDebug>
#include <Windows.h>
#include <CommCtrl.h>

App::App(const QString& id) :
    Entity(id)
{}

void App::exit() {
    QCoreApplication::quit();
}

void App::toggleHooks() {
    platform()->execute(WinToggleHooks);
}

void App::debugAction() {
    platform()->execute(WinUpdateCurrentToolTip);
}
