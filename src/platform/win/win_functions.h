#pragma once

#include <platform/platform_functions.h>

class WinFunctions : public PlatformFunctions {
    Q_OBJECT
public:
    WinFunctions(QObject* parent);

    virtual QSize consoleSize() const override;

    virtual void sendInput(const QString& text) const override;

    virtual void winUpdateCurrentToolTip() const override;

    virtual void winHideConsole() const override;
};
