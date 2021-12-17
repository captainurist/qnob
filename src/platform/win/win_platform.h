#pragma once

#include <memory>

#include <platform/platform.h>

class QThread;

class Com;
class WinSharedEventWindow;
class WinVolumeControl;
class WinMonitorManager;
class WinShortcutManager;
class WinWheelEventManager;
class WinMetrics;
class WinFunctions;

class WinPlatform: public Platform {
    Q_OBJECT
public:
    WinPlatform(QObject* parent);
    virtual ~WinPlatform();

    virtual PlatformVolumeControl* volumeControl() const override;
    virtual PlatformMonitorManager* monitorManager() const override;
    virtual PlatformShortcutManager* shortcutManager() const override;
    virtual PlatformWheelEventManager* wheelEventManager() const override;
    virtual PlatformMetrics* metrics() const override;
    virtual PlatformFunctions* functions() const override;
    virtual std::unique_ptr<PlatformControl> createStandardControl(PlatformStandardControl control, QObject* parent) const override;

private:
    std::unique_ptr<PlatformControl> createNativeOsdControl(QObject* parent) const;

private:
    std::unique_ptr<Com> m_com;
    std::unique_ptr<WinSharedEventWindow> m_eventWindow;
    std::unique_ptr<WinVolumeControl> m_volumeControl;
    std::unique_ptr<WinMonitorManager> m_monitorManager;
    std::unique_ptr<WinShortcutManager> m_shortcutManager;
    std::unique_ptr<WinWheelEventManager> m_wheelEventManager;
    std::unique_ptr<WinMetrics> m_metrics;
    std::unique_ptr<WinFunctions> m_functions;
};

