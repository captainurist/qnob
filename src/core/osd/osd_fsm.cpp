#include "osd_fsm.h"

#include <QtCore/QTimerEvent>
#include <QtCore/QVariantAnimation>
#include <QtCore/QPauseAnimation>
#include <QtCore/QSequentialAnimationGroup>

OsdFsm::OsdFsm() {
    m_waitAnimation = new QPauseAnimation(this);

    m_fadeAnimation = new QVariantAnimation(this);
    m_fadeAnimation->setStartValue(1.0);
    m_fadeAnimation->setEndValue(0.0);
    connect(m_fadeAnimation, &QVariantAnimation::valueChanged, this, [&](const QVariant& value) {
        setValue(value.toDouble());
    });

    m_groupAnimation = new QSequentialAnimationGroup(this);
    m_groupAnimation->addAnimation(m_waitAnimation);
    m_groupAnimation->addAnimation(m_fadeAnimation);
    connect(m_groupAnimation, &QAbstractAnimation::finished, this, &OsdFsm::finished);
}

void OsdFsm::start(int waitMs, int fadeMs) {
    if(m_groupAnimation->state() == QAbstractAnimation::Stopped)
        emit started();

    m_groupAnimation->stop();
    setValue(1.0);

    m_waitAnimation->setDuration(waitMs);
    m_fadeAnimation->setDuration(fadeMs);
    m_groupAnimation->start();
}

void OsdFsm::stop() {
    if (m_groupAnimation->state() == QAbstractAnimation::Stopped)
        return;

    m_groupAnimation->stop();
    setValue(0.0);

    emit finished();
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

