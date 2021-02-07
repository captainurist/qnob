#include "osd_window.h"

#include <QtGui/QPainter>
#include <QtCore/QDebug>

OsdWindow::OsdWindow() {
    setGeometry(100, 100, 300, 200);
    setFlag(Qt::WindowStaysOnTopHint);
    setFlag(Qt::FramelessWindowHint);
    setFlag(Qt::WindowTransparentForInput);
    setFlag(Qt::Tool);
    setOpacity(0.5);
}

void OsdWindow::setPicture(const DeferredPicture& picture) {
    m_picture = picture;
    update();
}

const DeferredPicture& OsdWindow::picture() const {
    return m_picture;
}

void OsdWindow::paintEvent(QPaintEvent* paintEvent) {
    QPainter painter(this);
    m_picture.play(&painter);
    painter.end();
}

