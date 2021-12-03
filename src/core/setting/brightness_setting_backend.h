#pragma once

#include <optional>

#include "setting_backend.h"

class MonitorManager;

class BrightnessSettingBackend : public SettingBackend {
    Q_OBJECT
public:
    BrightnessSettingBackend(MonitorManager* monitorManager, QObject* parent);
    virtual ~BrightnessSettingBackend();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool isEnabled() const override;
    virtual void setEnabled(bool value) override;

    virtual bool isInitialized() const override;

private:
    Q_SLOT void handleMonitorsChanged();

private:
    MonitorManager* m_monitorManager = nullptr;

    /** Whether we've successfully received the monitor list at least once. */
    bool m_initialized = false;

    /** Cached combined brightness, if any. */
    mutable std::optional<double> m_cachedValue;
};
