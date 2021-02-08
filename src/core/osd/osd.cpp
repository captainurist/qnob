#include "osd.h"

#include <util/deferred_picture.h>

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QRasterWindow>

#include "osd_window.h"
#include "osd_fsm.h"

Osd::Osd(const QString& id):
    Entity(id),
    m_window(new OsdWindow()),
    m_fsm(new OsdFsm())
{
    connect(m_fsm.get(), &OsdFsm::started, m_window.get(), &OsdWindow::show);
    connect(m_fsm.get(), &OsdFsm::valueChanged, m_window.get(), &OsdWindow::setOpacity);
    connect(m_fsm.get(), &OsdFsm::finished, m_window.get(), &OsdWindow::hide);
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
    m_window->resize(size);
}

QPoint Osd::position() const {
    return m_window->position();
}

void Osd::setPosition(QPoint point) {
    m_window->setPosition(point);
}

void Osd::update(const DeferredPicture& picture) {
    m_window->setPicture(picture);
}
