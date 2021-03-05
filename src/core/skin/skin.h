#pragma once

#include <memory>

#include <QtCore/QSize>

#include <core/entity/entity.h>

class SkinData;
class KnobPainter;

class Skin : public Entity {
    Q_OBJECT
public:
    Skin(const QString& id, const SkinData& data);

    QSize size() const;

    KnobPainter* createPainter() const;

private:
    std::shared_ptr<SkinData> m_data;
    QSize m_size;
};
