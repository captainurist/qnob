#include "win_physical_monitor.h"

#include "win_error.h"

class WinPhysicalMonitorPrivate {
public:
    std::vector<std::vector<PHYSICAL_MONITOR>> data;

    ~WinPhysicalMonitorPrivate() {
        for(auto& part: data)
            if (!part.empty())
                succeeded(DestroyPhysicalMonitors(part.size(), part.data()));
    }
};

WinPhysicalMonitor::WinPhysicalMonitor(PHYSICAL_MONITOR* monitor, std::shared_ptr<WinPhysicalMonitorPrivate> d) :
    m_monitor(monitor),
    m_d(d)
{}

BOOL CALLBACK EnumDisplayMonitorsProc(HMONITOR handle, HDC /*hdc*/, LPRECT /*rect*/, LPARAM data) {
    reinterpret_cast<std::vector<HMONITOR>*>(data)->push_back(handle);
    return TRUE;
}

std::vector<WinPhysicalMonitor> WinPhysicalMonitor::enumerateMonitors() {
    std::vector<WinPhysicalMonitor> result;

    std::vector<HMONITOR> monitors;
    if (!succeeded(EnumDisplayMonitors(nullptr, nullptr, EnumDisplayMonitorsProc, reinterpret_cast<LPARAM>(&monitors))))
        return result;
    if (monitors.size() == 0)
        return result;

    std::shared_ptr<WinPhysicalMonitorPrivate> d(new WinPhysicalMonitorPrivate());
    d->data.resize(monitors.size());

    for (size_t i = 0; i < monitors.size(); i++) {
        DWORD count = 0;
        BOOL status = TRUE;

        if (!succeeded(GetNumberOfPhysicalMonitorsFromHMONITOR(monitors[i], &count)))
            continue;
        if (count == 0)
            continue;
        d->data[i].resize(count);

        if (!succeeded(GetPhysicalMonitorsFromHMONITOR(monitors[i], count, d->data[i].data())))
            d->data[i].clear();
    }

    for (auto& part : d->data)
        for (PHYSICAL_MONITOR& handle : part)
            result.push_back(WinPhysicalMonitor(&handle, d));

    return result;
}

