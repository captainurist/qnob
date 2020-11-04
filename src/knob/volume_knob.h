#pragma once

#include "knob.h"

#include <QtCore/QScopedPointer>

class VolumeKnobPrivate;

class VolumeKnob : public Knob {
    Q_OBJECT
public:
    VolumeKnob(QObject* parent = nullptr);
    virtual ~VolumeKnob();

    virtual QByteArray id() const override;

    virtual int value() const override;
    virtual void setValue(int value) override;

    virtual bool enabled() const override;
    virtual void setEnabled(bool value) override;

private:
    QScopedPointer<VolumeKnobPrivate> d;
};
