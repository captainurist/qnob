#pragma once

#include <QtCore/QSize>

#include <platform/platform_metrics.h>

class WinSharedEventWindow;

class WinMetrics : public PlatformMetrics {
    Q_OBJECT
public:
    WinMetrics(WinSharedEventWindow* eventWindow, QObject* parent);

    virtual QSize trayIconSize() const override;

private:
    void updateTrayIconSize();

private:
    QSize m_cachedTrayIconSize;
};
