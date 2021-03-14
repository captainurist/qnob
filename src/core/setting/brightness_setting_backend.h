#pragma once

#include "setting_backend.h"

class BrightnessShaftPrivate;

class BrightnessSettingBackend : public SettingBackend {
    Q_OBJECT
public:
    BrightnessSettingBackend();
    virtual ~BrightnessSettingBackend();

    virtual double value() const override;
    virtual void setValue(double value) override;

    virtual bool isEnabled() const override;
    virtual void setEnabled(bool value) override;

    virtual bool isInitialized() const override;

private:
    Q_SLOT void handleFutureFinished();

private:
    std::unique_ptr<BrightnessShaftPrivate> d;
};
