#pragma once

#include <wtypes.h>

#include <memory>
#include <unordered_set>

#include <platform/platform_wheel_event_manager.h>

typedef struct tagMSG MSG;
class QSystemTrayIcon;
class WinSharedEventWindow;
class PlatformControl;

class WinWheelEventManager: public PlatformWheelEventManager {
    Q_OBJECT
public:
    WinWheelEventManager(WinSharedEventWindow* eventWindow);
    virtual ~WinWheelEventManager();

    virtual void registerTrayIcon(QSystemTrayIcon* icon) override;
    virtual void unregisterTrayIcon(QSystemTrayIcon* icon) override;

    virtual void registerControl(PlatformControl* control) override;
    virtual void unregisterControl(PlatformControl* control) override;

private:
    Q_SLOT void processInput(MSG* message);

private:
    std::unordered_set<QSystemTrayIcon*> m_icons;
    std::unordered_set<PlatformControl*> m_controls;
};
