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

void App::debugAction() {
    platform()->execute(WinUpdateCurrentToolTip);

    qDebug() << "Debug action.";
}
