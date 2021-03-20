#include "app.h"

#include <QtCore/QCoreApplication>

#include <platform/platform.h>

App::App(const QString& id) :
    Entity(id)
{}

void App::exit() {
    QCoreApplication::quit();
}

void App::toggleHooks() {
    platform()->setHooksEnabled(!platform()->hooksEnabled());
}
