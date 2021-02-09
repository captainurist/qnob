#include "osd_window.h"

#include <QtGui/QPainter>
#include <QtCore/QDebug>

OsdWindow::OsdWindow() {
    setFlag(Qt::WindowStaysOnTopHint);
    setFlag(Qt::FramelessWindowHint);
    setFlag(Qt::WindowTransparentForInput);
    setFlag(Qt::Tool);

    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    setFormat(format);

    resize(0, 0);
}

void OsdWindow::setPicture(const DeferredPicture& picture) {
    m_picture = picture;
    update();
}

const DeferredPicture& OsdWindow::picture() const {
    return m_picture;
}

void OsdWindow::paintEvent(QPaintEvent* /*paintEvent*/) {
    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(QRect(QPoint(), size()), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    m_picture.play(&painter);
    painter.end();
}

