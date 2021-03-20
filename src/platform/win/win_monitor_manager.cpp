#include "win_monitor_manager.h"

#include <physicalmonitorenumerationapi.h> /* For EnumDisplayMonitors. */

#include <cassert>

#include <QtCore/QUuid>
#include <QtCore/QVarLengthArray>
#include <QtCore/QDebug>
#include <QtGui/QWindow>

#include "win_monitor.h"
#include "win_error.h"
#include "win_native_event_window.h"

struct MonitorInfo {
    DISPLAY_DEVICEW display;
    DISPLAY_DEVICEW monitor;
    size_t monitorIndex;

    MonitorInfo() {
        display.cb = monitor.cb = sizeof(DISPLAY_DEVICEW);
    }
};

struct DisplayInfo {
    HMONITOR handle;
    MONITORINFOEXW display;

    DisplayInfo() {
        display.cbSize = sizeof(MONITORINFOEXW);
    }
};

static BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR handle, HDC /*hdc*/, LPRECT /*rect*/, LPARAM data) {
    DisplayInfo element;
    element.handle = handle;

    if(!apicall(GetMonitorInfoW(handle, &element.display)))
        return TRUE;

    reinterpret_cast<std::vector<DisplayInfo>*>(data)->push_back(element);
    return TRUE;
}

static std::vector<DisplayInfo> enumDisplays() {
    std::vector<DisplayInfo> result;
    if (!apicall(EnumDisplayMonitors(nullptr, nullptr, EnumDisplayMonitorsProc, reinterpret_cast<LPARAM>(&result))))
        result.clear();
    return result;
}

static std::vector<MonitorInfo> enumMonitors() {
    std::vector<MonitorInfo> result;

    MonitorInfo element;
    for (size_t i = 0; EnumDisplayDevicesW(NULL, i, &element.display, EDD_GET_DEVICE_INTERFACE_NAME); i++) {
        if (element.display.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER)
            continue; /* Represents a pseudo device used to mirror application drawing for remoting. */

        for (uint j = 0; EnumDisplayDevicesW(element.display.DeviceName, j, &element.monitor, EDD_GET_DEVICE_INTERFACE_NAME); j++) {
            /* In theory we should check here that DISPLAY_DEVICE_ACTIVE is set.
             * However, starting with Vista it's always set. And Qt6 only supports Windows 10+. */

            element.monitorIndex = j;
            result.push_back(element);
        }
    }

    return result;
}

static QString monitorDeviceId(const std::vector<MonitorInfo>& monitorInfos, const DisplayInfo& displayInfo, size_t monitorIndex, const PHYSICAL_MONITOR& physicalMonitor) {
    for (const MonitorInfo& monitorInfo : monitorInfos) {
        if (monitorInfo.monitorIndex != monitorIndex ||
            std::wcscmp(monitorInfo.display.DeviceName, displayInfo.display.szDevice) != 0 ||
            std::wcscmp(monitorInfo.monitor.DeviceString, physicalMonitor.szPhysicalMonitorDescription) != 0)
            continue;

        /* DeviceId below usually looks like this:
         *
         * \\?\DISPLAY#<hwid>#<instanceid>#{e6f07b5f-ee97-4a90-b076-33f57bf4eaa7}
         *
         * ({e6f07b5f-ee97-4a90-b076-33f57bf4eaa7} means GUID_DEVINTERFACE_MONITOR.)
         *
         * We normalize this to look the same as 'device instance path' property in device manager:
         *
         * DISPLAY\<hwid>\<instanceid> */
        QString deviceId = QString::fromWCharArray(monitorInfo.monitor.DeviceID);
        QStringList chunks = deviceId.split(QLatin1Char('#'));
        if (chunks.size() < 3 || chunks[0] != lit("\\\\?\\DISPLAY")) {
            qWarning() << "Invalid monitor deviceId format:" << deviceId;
            continue;
        }

        chunks.resize(3);
        chunks[0] = lit("DISPLAY");
        return chunks.join(QLatin1Char('\\'));
    }

    return QString();
}

WinMonitorManager::WinMonitorManager():
    m_eventWindow(new WinNativeEventWindow(lit("WinMonitorManagerEventWindow"), WM_DISPLAYCHANGE))
{
    /* This magic here is needed because message-only windows don't receive broadcast messages, see
     * https://stackoverflow.com/questions/22570008/not-receiving-wm-displaychange */
    m_eventWindow->setFlag(Qt::FramelessWindowHint);
    m_eventWindow->setFlag(Qt::WindowTransparentForInput);
    m_eventWindow->setGeometry(0, 0, 0, 0);
    m_eventWindow->show(); /* This is where the underlying window is created. */
    m_eventWindow->hide();

    connect(m_eventWindow.get(), &WinNativeEventWindow::messageReceived, this, &WinMonitorManager::dispatchEvent);
}

WinMonitorManager::~WinMonitorManager() {}

std::vector<std::unique_ptr<PlatformMonitor>> WinMonitorManager::enumerateMonitors() {
    std::vector<std::unique_ptr<PlatformMonitor>> result;

    std::vector<MonitorInfo> monitorInfos = enumMonitors();

    QVarLengthArray<PHYSICAL_MONITOR, 8> physicalMonitors;
    for (DisplayInfo displayInfo : enumDisplays()) {
        DWORD count = 0;

        if (!apicall(GetNumberOfPhysicalMonitorsFromHMONITOR(displayInfo.handle, &count)) || count == 0)
            continue;

        physicalMonitors.resize(count);

        if (!apicall(GetPhysicalMonitorsFromHMONITOR(displayInfo.handle, count, physicalMonitors.data())))
            continue;

        for (size_t monitorIndex = 0; monitorIndex < physicalMonitors.size(); monitorIndex++) {
            QString deviceId = monitorDeviceId(monitorInfos, displayInfo, monitorIndex, physicalMonitors[monitorIndex]);
            if (deviceId.isEmpty()) {
                /* Just generate a random deviceId so that all the machinery down the line still works.
                 * In theory, we should never get here. */
                deviceId = lit("DISPLAY\\%1\\{e6f07b5f-ee97-4a90-b076-33f57bf4eaa7}").arg(QUuid::createUuid().toString());
            }

            result.emplace_back(new WinMonitor(deviceId, physicalMonitors[monitorIndex]));
        }
    }

    return result;
}

void WinMonitorManager::dispatchEvent(void* message) {
    MSG* msg = static_cast<MSG*>(message);
    assert(msg->message == WM_DISPLAYCHANGE);

    emit monitorsChanged();
}
