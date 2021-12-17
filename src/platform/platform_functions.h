#pragma once

#include <QtCore/QObject>
#include <QtCore/QSize>

class PlatformFunctions : public QObject {
    Q_OBJECT
public:
    PlatformFunctions(QObject* parent) : QObject(parent) {}

    /**
     * \returns                         Console size, or invalid QSize in case of an error.
     */
    virtual QSize consoleSize() const = 0;

    virtual void sendInput(const QString& text) const = 0;

    virtual void winUpdateCurrentToolTip() const = 0;

    virtual void winHideConsole() const = 0;
};
