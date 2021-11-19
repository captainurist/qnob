#pragma once

#include <core/entity/entity.h>

class QPainter;
class SettingState;

class Skin : public Entity {
    Q_OBJECT
public:
    using Entity::Entity;

    /**
     * \returns                         Expected size for `paint` calls. Can be invalid.
     */
    virtual QSize size() const = 0;

    /**
     * Draws the skin using the provided painter in the provided rect.
     *
     * \param painter                   Painter to use.
     * \param size                      Requested size. Note that it might not be equal to what `size()` has returned.
     * \param state                     Knob state to draw.
     */
    virtual void paint(QPainter* painter, const QSize& size, const SettingState& state) = 0;
};
