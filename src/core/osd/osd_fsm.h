#pragma once

#include <QtCore/QObject>

class QVariantAnimation;

class OsdFsm : public QObject {
    Q_OBJECT
public:
    OsdFsm();

    void start(int waitMs, int fadeMs);
    void stop();

    double value() const;
    void setValue(double value);

signals:
    void started();
    void valueChanged(double value);
    void finished();

private:
    double m_value = 0.0;
    QVariantAnimation* m_showAnimation;
    QVariantAnimation* m_fadeAnimation;
};

