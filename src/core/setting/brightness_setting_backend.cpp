#include "brightness_setting_backend.h"

#include <cassert>
#include <optional>

#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>

#include <platform/platform.h>
#include <platform/platform_monitor.h>
#include <lib/monitor/monitor_manager.h>

using MonitorList = std::vector<std::unique_ptr<PlatformMonitor>>;

class BrightnessShaftPrivate {
public:
    /** Pending monitor list, updated on startup and when system monitors change. */
    QFuture<MonitorList> future;

    /** Current monitor list. */
    MonitorList monitors;

    /** Whether we've successfully received the monitor list at least once. */
    bool initialized = false;

    /** Cached combined brightness, if any. */
    std::optional<double> cachedValue;
};

BrightnessSettingBackend::BrightnessSettingBackend():
    d(new BrightnessShaftPrivate)
{
    updateMonitorList();

    connect(platform()->monitorManager(), &PlatformMonitorManager::monitorsChanged, this, &BrightnessSettingBackend::updateMonitorList);
}

BrightnessSettingBackend::~BrightnessSettingBackend() {
    if (d->future.isStarted()) {
        d->future.cancel();
        d->future.waitForFinished();
    }
}

double BrightnessSettingBackend::value() const {
    if (!d->initialized)
        return NAN;

    if (d->cachedValue)
        return *d->cachedValue;

    double result = 0;
    for (auto& monitor : d->monitors)
        result += monitor->property(BrightnessMonitorProperty);
    result /= d->monitors.size();

    assert(!std::isnan(result));

    d->cachedValue = result;
    return result;
}

void BrightnessSettingBackend::setValue(double value) {
    assert(value >= 0.0 && value <= 1.0);

    if (!d->initialized)
        return;

    d->cachedValue = value;
    for (auto& monitor : d->monitors)
        monitor->setProperty(BrightnessMonitorProperty, value);
}

bool BrightnessSettingBackend::isEnabled() const {
    return true;
}

void BrightnessSettingBackend::setEnabled(bool /*value*/) {
    return; /* Not supported by the underlying API. */
}

bool BrightnessSettingBackend::isInitialized() const {
    return d->initialized;
}

void BrightnessSettingBackend::updateMonitorList() {
    d->future = MonitorManager::enumerateMonitors(platform()->monitorManager());

    QFutureWatcher<MonitorList>* watcher = new QFutureWatcher<MonitorList>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, &BrightnessSettingBackend::handleFutureFinished);
    connect(watcher, &QFutureWatcherBase::finished, watcher, &QObject::deleteLater);
    watcher->setFuture(d->future);
}

void BrightnessSettingBackend::handleFutureFinished() {
    if (!d->future.isFinished())
        return; /* Got canceled or replaced with a new future because monitors changed. */

    d->monitors = d->future.takeResult();

    if (!d->initialized) {
        d->initialized = true;
        emit initialized();
    }
}
