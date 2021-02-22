#include "osd.h"

#include <util/deferred_picture.h>

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QRasterWindow>
#include <QtGui/QScreen>

#include "osd_window.h"
#include "osd_fsm.h"

Osd::Osd(const QString& id):
    Entity(id),
    m_window(new OsdWindow()),
    m_fsm(new OsdFsm())
{
    connect(m_fsm.get(), &OsdFsm::started, m_window.get(), &OsdWindow::show);
    connect(m_fsm.get(), &OsdFsm::started, m_window.get(), &OsdWindow::raise);
    connect(m_fsm.get(), &OsdFsm::valueChanged, m_window.get(), &OsdWindow::setOpacity);
    connect(m_fsm.get(), &OsdFsm::finished, m_window.get(), &OsdWindow::hide);

    /* Note that connection to raise above is important. Otherwise OSD window ends up below all other windows after
     * changing main screen in Win10. */
}

Osd::~Osd() {}

void Osd::show(int showMs, int fadeMs) {
    m_fsm->start(showMs, fadeMs);
}

void Osd::hide() {
    m_fsm->stop();
}

QSize Osd::size() const {
    return m_window->size();
}

void Osd::resize(QSize size) {
    if (size == m_window->size())
        return;

    m_window->resize(size);
    updatePosition();
}

Qt::Alignment Osd::alignment() const {
    return m_alignment;
}

void Osd::setAlignment(Qt::Alignment alignment) {
    if (m_alignment == alignment)
        return;

    m_alignment = alignment;
    updatePosition();
}

QPoint Osd::offset() const {
    return m_offset;
}

void Osd::setOffset(const QPoint& offset) {
    if (m_offset == offset)
        return;

    m_offset = offset;
    updatePosition();
}

void Osd::update(const DeferredPicture& picture) {
    m_window->setPicture(picture);
}

void Osd::updatePosition() {
    QSize screenSize = m_window->screen()->size();
    QSize windowSize = m_window->size();

    QPoint position;

    if (m_alignment & Qt::AlignLeft) {
        position.setX(0);
    } else if (m_alignment & Qt::AlignRight) {
        position.setX(screenSize.width() - windowSize.width());
    } else {
        position.setX((screenSize.width() - windowSize.width()) / 2);
    }

    if (m_alignment & Qt::AlignTop) {
        position.setY(0);
    } else if (m_alignment & Qt::AlignBottom) {
        position.setY(screenSize.height() - windowSize.height());
    } else {
        position.setY((screenSize.height() - windowSize.height()) / 2);
    }

    m_window->setPosition(position + m_offset);
}