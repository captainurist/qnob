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

    virtual void paint(QPainter* painter, const QSize& size, const SettingState& state) {
        QString text = state.enabled ? QString::number(static_cast<int>(std::round(100 * state.value))) : lit("0");

        PainterTransformRollback transformRollback(painter);
        PainterPenRollback penRollback(painter, m_color);
        PainterFontRollback fontRollback(painter);
        QFont font = painter->font();
        if (!m_fontFamily.isEmpty())
            font.setFamily(m_fontFamily);
        font.setPixelSize(findFontPixelSize(font, text, size));
        painter->setFont(font);

        painter->drawText(QRect(QPoint(0, 0), size), Qt::AlignCenter, text);
    }

private:
    static QSize advanceAscend(const QFont& font, const QString& text) {
        QFontMetrics metrics(font);
        return QSize(metrics.horizontalAdvance(text), metrics.ascent());
    }

    static bool fitsInto(const QSize& small, const QSize& big) {
        return small.width() <= big.width() && small.height() <= big.width();
    }

    static int findFontPixelSize(QFont font, const QString& text, const QSize& boundingSize) {
        /* Binary search in [l, r). */
        int l = 1;
        int r = std::max(boundingSize.width(), boundingSize.height()) + 1;

        while (l + 1 < r) {
            int m = (l + r) / 2;
            font.setPixelSize(m);

            if (fitsInto(advanceAscend(font, text), boundingSize)) {
                l = m;
            } else {
                r = m;
            }
        }

        return l;
    }

private:
    QSize m_size;
    QColor m_color;
    QString m_fontFamily;
};
