#include "brightness_shaft.h"

#include <cmath>
#include <cassert>

#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>

#include <platform/platform.h>
#include <platform/platform_monitor.h>
#include <lib/monitor/monitor_manager.h>

using MonitorList = std::vector<std::unique_ptr<PlatformMonitor>>;

class BrightnessShaftPrivate {
public:
    std::unique_ptr<PlatformMonitorManager> monitorManager;
    QFuture<MonitorList> future;
    MonitorList monitors;
    bool initialized = false;
    double value = NAN;
};

BrightnessShaft::BrightnessShaft():
    d (new BrightnessShaftPrivate)
{
    d->monitorManager.reset(platform()->createMonitorManager());
    d->future = MonitorManager::enumerateMonitors(d->monitorManager.get());

    QFutureWatcher<MonitorList>* watcher = new QFutureWatcher<MonitorList>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, &BrightnessShaft::handleFutureFinished);
    watcher->setFuture(d->future);
}

BrightnessShaft::~BrightnessShaft() {
    if (d->initialized)
        return;

    d->future.cancel();
    d->future.waitForFinished();
}

void BrightnessShaft::handleFutureFinished() {
    if (!d->future.isValid())
        return; /* Got canceled. */

    d->monitors = d->future.takeResult();
    d->initialized = true;
}

double BrightnessShaft::value() const {
    if (!d->initialized)
        return 0.0;

    double result = d->value;

    if (std::isnan(result)) {
        result = 0;
        for (auto& monitor : d->monitors)
            result += monitor->property(PlatformMonitor::BrightnessProperty);
        result /= d->monitors.size();

        if (std::isnan(result))
            result = 0.0;
    }

    return result;
}

void BrightnessShaft::setValue(double value) {
    assert(value >= 0.0 && value <= 1.0);

    if (!d->initialized)
        return;

    d->value = value;
    for (auto& monitor : d->monitors)
        monitor->setProperty(PlatformMonitor::BrightnessProperty, value);
}

bool BrightnessShaft::enabled() const {
    return true;
}

void BrightnessShaft::setEnabled(bool /*value*/) {
    return; /* Just ignore it for now. */
}

