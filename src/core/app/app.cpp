#include "app.h"

#include <QtCore/QCoreApplication>

#include <platform/platform.h>

#include <QtCore/QDebug>

App::App(const QString& id) :
    Entity(id)
{}

void App::exit() {
    QCoreApplication::quit();
}

void App::disableHooks() {
    qDebug() << "Hooks disabled.";
    platform()->execute(WinDisableHooks);
}

void App::debugAction() {
    qDebug() << "Debug action.";
    platform()->execute(WinUpdateCurrentToolTip);
}
