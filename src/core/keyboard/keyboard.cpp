#include "keyboard.h"

#include <platform/platform.h>

Keyboard::Keyboard(QObject* parent) :
    Entity(parent)
{}

Q_INVOKABLE void Keyboard::type(const QString& text) {
    platform()->functions()->sendInput(text);
}
