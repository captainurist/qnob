#pragma once

#include "setting_backend.h"

class VolumeSettingBackend : public SettingBackend {
    Q_OBJECT
public:
    VolumeSettingBackend();
    virtual ~VolumeSettingBackend();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool isEnabled() const override;
    virtual void setEnabled(bool value) override;

    virtual bool isInitialized() const override;
};
