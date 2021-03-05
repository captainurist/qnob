#include "skin.h"

#include <QtGui/QPainter>

#include <core/knob/knob_state.h>

#include "skin_data.h"

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

void Skin::paint(QPainter* painter, const KnobState& state) {
    if (state.enabled) {
        painter->drawPixmap(0, 0, m_data.background);

        QRect sourceRect(0, 0, m_data.progress.width() * state.value, m_data.progress.height());
        if (sourceRect.width() > 0)
            painter->drawPixmap(m_data.offset, m_data.progress, sourceRect);
    } else {
        painter->drawPixmap(0, 0, m_data.disabled);
    }
}

