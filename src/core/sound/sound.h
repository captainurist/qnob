#pragma once

#include <core/entity/entity.h>

class Sound : public Entity {
    Q_OBJECT
public:
    Sound(const QString& id, const QString& path);

    Q_SLOT void play();

private:
    QString m_path;
};
