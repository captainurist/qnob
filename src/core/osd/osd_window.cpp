#include "osd_window.h"

#include <QtGui/QPainter>
#include <QtCore/QDebug>

#include <core/skin/skin.h>

OsdWindow::OsdWindow(const QString& title) {
    setFlag(Qt::WindowStaysOnTopHint);
    setFlag(Qt::FramelessWindowHint);
    setFlag(Qt::WindowTransparentForInput);
    setFlag(Qt::Tool);
    setFlag(Qt::MSWindowsFixedSizeDialogHint); /* Prevents automatic resizes on WM_DPICHANGED. */
    setTitle(title); /* Mainly used for debugging with Spy++. */

    QSurfaceFormat format;
    format.setAlphaBufferSize(8);
    setFormat(format);

    resize(0, 0);
}

SettingState OsdWindow::state() const {
    return m_state;
}

void OsdWindow::setState(const SettingState& state) {
    if (m_state == state)
        return;

    m_state = state;

    update();
}

Skin* OsdWindow::skin() const {
    return m_skin;
}

void OsdWindow::setSkin(Skin* skin) {
    m_skin = skin;

    resize(m_skin ? m_skin->size() : QSize());
}

void OsdWindow::paintEvent(QPaintEvent* /*paintEvent*/) {
    if (!m_skin)
        return;

    QPainter painter(this);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(QRect(QPoint(), size()), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    m_skin->paint(&painter, size(), m_state);
    painter.end();
}

