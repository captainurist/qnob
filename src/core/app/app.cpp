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

void App::toggleHooks() {
    bool newState = !platform()->execute(WinHooksEnabled).toBool();
    platform()->execute(WinSetHooksEnabled, newState);

    qDebug() << (newState ? "Hooks enabled." : "Hooks disabled.");
}

void App::debugAction() {
    platform()->execute(WinUpdateCurrentToolTip);

    qDebug() << "Debug action.";
}
