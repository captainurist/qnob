#pragma once

#include <QtCore/QObject>

/**
 * This class is mainly used for debugging.
 */
class WinThreadMessageHook : public QObject {
    Q_OBJECT;
public:
    WinThreadMessageHook();
    virtual ~WinThreadMessageHook();

signals:
    void serious();
};
