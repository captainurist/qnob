#include "osd_fsm.h"

#include <QtCore/QTimerEvent>
#include <QtCore/QVariantAnimation>

OsdFsm::OsdFsm() {
    auto setVariantValue = [&](const QVariant& value) {
        setValue(value.toDouble());
    };

    m_showAnimation = new QVariantAnimation(this);
    m_showAnimation->setStartValue(1.0);
    m_showAnimation->setEndValue(1.0);
    connect(m_showAnimation, &QVariantAnimation::valueChanged, this, setVariantValue);

    m_fadeAnimation = new QVariantAnimation(this);
    m_fadeAnimation->setStartValue(1.0);
    m_fadeAnimation->setEndValue(0.0);
    connect(m_fadeAnimation, &QVariantAnimation::valueChanged, this, setVariantValue);
    connect(m_fadeAnimation, &QAbstractAnimation::finished, this, &OsdFsm::finished);

    connect(m_showAnimation, &QAbstractAnimation::finished, m_fadeAnimation, [&] { m_fadeAnimation->start(); });
}

void OsdFsm::start(int waitMs, int fadeMs) {
    if(m_showAnimation->state() == QAbstractAnimation::Stopped && m_fadeAnimation->state() == QAbstractAnimation::Stopped)
        emit started();

    m_showAnimation->stop();
    m_fadeAnimation->stop();
    setValue(1.0);

    m_showAnimation->setDuration(waitMs);
    m_fadeAnimation->setDuration(fadeMs);

    m_showAnimation->start();
}

void OsdFsm::stop() {
    m_showAnimation->stop();
    m_fadeAnimation->stop();
    setValue(0.0);
}

double OsdFsm::value() const {
    return m_value;
}

void OsdFsm::setValue(double value) {
    if (qFuzzyCompare(m_value, value))
        return;

    m_value = value;

    emit valueChanged(value);
}

