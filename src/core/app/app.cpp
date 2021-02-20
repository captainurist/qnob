#include "app.h"

#include <QtCore/QCoreApplication>

App::App(const QString& id) :
    Entity(id)
{}

void App::exit() {
    QCoreApplication::quit();
}
