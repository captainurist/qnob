#include "osd_window.h"

#include <QtGui/QPainter>
#include <QtCore/QDebug>

#include <core/painting/knob_painter.h>

OsdWindow::OsdWindow(const QString& title) {
    setFlag(Qt::WindowStaysOnTopHint);
    setFlag(Qt::FramelessWindowHint);
    setFlag(Qt::WindowTransparentForInput);
    setFlag(Qt::Tool);
    setTitle(title); /* Mainly used for debugging with Spy++. */

    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    setFormat(format);

    resize(0, 0);
}

KnobState OsdWindow::state() const {
    return m_state;
}

void OsdWindow::setState(const KnobState& state) {
    if (m_state == state)
        return;

    m_state = state;

    update();
}

void OsdWindow::setPainter(KnobPainter* painter) {
    m_painter.reset(painter);
}

void OsdWindow::paintEvent(QPaintEvent* /*paintEvent*/) {
    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(QRect(QPoint(), size()), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    m_painter->paint(&painter, m_state);
    painter.end();
}

