#pragma once

#include <QtGui/QPixmap>
#include <QtGui/QPainter>

#include <lib/rollback/painter_rollback.h>

#include <core/setting/setting_state.h>

#include "skin.h"

class HorizontalBarSkin: public Skin {
    Q_OBJECT
public:
    HorizontalBarSkin(const QString& id, const QPixmap& background, const QPixmap& progress, const QPixmap& disabled, const QPoint& offset) :
        Skin(id),
        m_background(background),
        m_progress(progress),
        m_disabled(disabled),
        m_offset(offset)
    {}

    virtual QSize size() const override {
        return m_background.size();
    }

    virtual void paint(QPainter* painter, const QSize& size, const SettingState& state) {
        PainterTransformRollback rollback(painter);
        painter->scale(static_cast<qreal>(size.width()) / m_background.width(), static_cast<qreal>(size.height()) / m_background.height());

        if (state.enabled) {
            painter->drawPixmap(0, 0, m_background);

            QRect sourceRect(0, 0, m_progress.width() * state.value, m_progress.height());
            if (sourceRect.width() > 0)
                painter->drawPixmap(m_offset, m_progress, sourceRect);
        } else {
            painter->drawPixmap(0, 0, m_disabled);
        }
    }

private:
    QPixmap m_background;
    QPixmap m_progress;
    QPixmap m_disabled;
    QPoint m_offset;
};
