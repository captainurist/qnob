#pragma once

#include <memory>

#include <QtCore/QSize>

#include <core/entity/entity.h>

#include "skin_data.h"

class QPainter;
class KnobState;

class Skin : public Entity {
    Q_OBJECT
public:
    Skin(const QString& id, const SkinData& data);

    QSize size() const;

    void paint(QPainter* painter, const KnobState& state);

private:
    SkinData m_data;
    QSize m_size;
};
