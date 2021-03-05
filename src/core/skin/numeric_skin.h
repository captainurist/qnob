#pragma once

#include <QtGui/QPainter>

#include <core/knob/knob_state.h>

#include "skin.h"

class NumericSkin : public Skin {
    Q_OBJECT
public:
    NumericSkin(const QString& id, const QColor& color) :
        Skin(id),
        m_color(color)
    {}

    virtual QSize size() const override {
        return QSize(64, 64);
    }

    virtual void paint(QPainter* painter, const KnobState& state) {
        QString text = state.enabled ? QString::number(static_cast<int>(std::round(100 * state.value))) : lit("0");

        QFont font = painter->font();
        font.setPixelSize(64);
        painter->setFont(font);
        painter->setPen(m_color); // TODO: rollback!

        QFontMetrics metrics(font);
        QRect rect = metrics.boundingRect(text);
        if (rect.width() > 64) {
            painter->translate(32, 0);
            painter->scale(64.0 / rect.width(), 1.0);
            painter->translate(-32, 0);
        }

        painter->drawText(QRect(-32, 0, 128, 64), Qt::AlignCenter, text);
    }

private:
    QColor m_color;
};
