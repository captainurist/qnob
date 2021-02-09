#include "skin.h"

#include <QtGui/QPainter>

#include <core/knob/knob_state.h>
#include <util/deferred_picture.h>

#include "skin_data.h"

Skin::Skin(const QString& id, const SkinData& data) :
    Entity(id),
    m_data(new SkinData(data))
{
    m_size.setWidth(std::max(m_data->background.width(), m_data->disabled.width()));
    m_size.setHeight(std::max(m_data->background.height(), m_data->disabled.height()));
}

QSize Skin::size() const {
    return m_size;
}

DeferredPicture Skin::picture(const KnobState& state) const {
    return [data=m_data, state](QPainter* painter) {
        if (state.enabled) {
            painter->drawPixmap(0, 0, data->background);

            QRect sourceRect(0, 0, data->progress.width() * state.value, data->progress.height());
            if (sourceRect.width() > 0)
                painter->drawPixmap(data->offset, data->progress, sourceRect);
        } else {
            painter->drawPixmap(0, 0, data->disabled);
        }
    };
}
