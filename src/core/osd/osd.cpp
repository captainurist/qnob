#include "osd.h"

#include <util/deferred_picture.h>

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QRasterWindow>

#include "osd_window.h"

Osd::Osd(const QString& id):
    Entity(id)
{
    m_window.reset(new OsdWindow());
    m_window->setGeometry(100, 100, 300, 200);
    m_window->setFlag(Qt::WindowStaysOnTopHint);
    m_window->setFlag(Qt::FramelessWindowHint);
    m_window->setFlag(Qt::WindowTransparentForInput);
    m_window->setFlag(Qt::Tool);
    m_window->setOpacity(0.5);
    m_window->show();
}

Osd::~Osd() {}

void Osd::update(const DeferredPicture& picture) {
    m_window->setPicture(picture);
}
