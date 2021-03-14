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
    QFuture<MonitorList> future;
    MonitorList monitors;
    bool initialized = false;
    std::optional<double> cachedValue;
};

BrightnessSettingBackend::BrightnessSettingBackend():
    d(new BrightnessShaftPrivate)
{
    d->future = MonitorManager::enumerateMonitors(platform()->monitorManager());

    QFutureWatcher<MonitorList>* watcher = new QFutureWatcher<MonitorList>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, &BrightnessSettingBackend::handleFutureFinished);
    watcher->setFuture(d->future);
}

BrightnessSettingBackend::~BrightnessSettingBackend() {
    if (d->initialized)
        return;

    d->future.cancel();
    d->future.waitForFinished();
}

double BrightnessSettingBackend::value() const {
    if (!d->initialized)
        return NAN;

    if (d->cachedValue)
        return *d->cachedValue;

    double result = 0;
    for (auto& monitor : d->monitors)
        result += monitor->property(PlatformMonitor::BrightnessProperty);
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
        monitor->setProperty(PlatformMonitor::BrightnessProperty, value);
}

bool BrightnessSettingBackend::isEnabled() const {
    return d->initialized;
}

void BrightnessSettingBackend::setEnabled(bool /*value*/) {
    return; /* Not supported by the underlying API. */
}

bool BrightnessSettingBackend::isInitialized() const {
    return d->initialized;
}

void BrightnessSettingBackend::handleFutureFinished() {
    if (!d->future.isValid())
        return; /* Got canceled. */

    d->monitors = d->future.takeResult();
    d->initialized = !d->monitors.empty();

    if (d->initialized)
        emit initialized();
}
