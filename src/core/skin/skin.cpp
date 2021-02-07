#include "skin.h"

#include <QtGui/QPainter>

#include <core/knob/knob_state.h>
#include <util/deferred_picture.h>

#include <QtGui/QPicture>

Skin::Skin(const QString& id, const SkinData& data) :
    Entity(id),
    m_data(data)
{
    m_size.setWidth(std::max(m_data.background.width(), m_data.disabled.width()));
    m_size.setHeight(std::max(m_data.background.height(), m_data.disabled.height()));
}

QSize Skin::size() const {
    return m_size;
}

DeferredPicture Skin::picture(const KnobState& state) const {
    // TODO: `this` is still by ref? uuuh
    return [=](QPainter* painter) {
        if (state.enabled) {
            painter->drawPixmap(0, 0, m_data.background);
        } else {
            painter->drawPixmap(0, 0, m_data.disabled);
        }
    };
}
