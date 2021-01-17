#include "volume_knob.h"

VolumeKnob::VolumeKnob(QObject* parent) : 
    Knob(parent),
    d(new VolumeKnobPrivate)
{
    d->q = this;
    d->init();
}

VolumeKnob::~VolumeKnob() 
{}

QByteArray VolumeKnob::id() const {
    return "volume";
}

int VolumeKnob::value() const {
    return d->value();
}

void VolumeKnob::setValue(int value) {
    d->setValue(value);
}

bool VolumeKnob::enabled() const {
    return d->enabled();
}

void VolumeKnob::setEnabled(bool value) {
    d->setEnabled(value);
}

