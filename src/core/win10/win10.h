#pragma once

#include <core/entity/entity.h>

class Win10 : public Entity {
    Q_OBJECT
public:
    Win10(const QString& id);
    virtual ~Win10();

    Q_INVOKABLE void ensureNativeOsdHidden();

private:
    bool m_systemOsdHidden = false;
};
