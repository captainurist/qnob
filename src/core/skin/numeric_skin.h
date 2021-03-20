#pragma once

#include <QtGui/QPainter>

#include <lib/rollback/painter_rollback.h>
#include <core/setting/setting_state.h>

#include "skin.h"

class NumericSkin : public Skin {
    Q_OBJECT
public:
    NumericSkin(const QString& id, const QSize& size, const QColor& color, const QString& fontFamily = QString()) :
        Skin(id),
        m_size(size),
        m_color(color),
        m_fontFamily(fontFamily)
    {}

    virtual QSize size() const override {
        return m_size;
    }

    virtual void paint(QPainter* painter, const SettingState& state) {
        QString text = state.enabled ? QString::number(static_cast<int>(std::round(100 * state.value))) : lit("0");

        PainterTransformRollback transformRollback(painter);
        PainterPenRollback penRollback(painter, m_color);
        PainterFontRollback fontRollback(painter);
        QFont font = painter->font();
        font.setPixelSize(m_size.height());
        if(!m_fontFamily.isEmpty())
            font.setFamily(m_fontFamily);
        painter->setFont(font);

        QFontMetrics metrics(font);
        QRect rect = metrics.boundingRect(text);
        if (rect.width() > m_size.width()) {
            painter->translate(m_size.width() / 2.0, 0);
            painter->scale(1.0 * m_size.width() / rect.width(), 1.0);
            painter->translate(-m_size.width() / 2.0, 0);
        }

        painter->drawText(QRect(-m_size.width() / 2.0, 0, m_size.width() * 2.0, m_size.height()), Qt::AlignCenter, text);
    }

private:
    QSize m_size;
    QColor m_color;
    QString m_fontFamily;
};
