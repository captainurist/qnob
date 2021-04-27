#include "osd.h"

#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QtGui/QRasterWindow>
#include <QtGui/QScreen>

#include <core/setting/setting.h>

#include "osd_window.h"
#include "osd_fsm.h"

Osd::Osd(const QString& id):
    Entity(id),
    m_window(new OsdWindow(lit("OsdWindow"))),
    m_fsm(new OsdFsm())
{
    connect(m_fsm.get(), &OsdFsm::started, m_window.get(), &OsdWindow::show);
    connect(m_fsm.get(), &OsdFsm::started, m_window.get(), &OsdWindow::raise);
    connect(m_fsm.get(), &OsdFsm::valueChanged, m_window.get(), &OsdWindow::setOpacity);
    connect(m_fsm.get(), &OsdFsm::finished, m_window.get(), &OsdWindow::hide);

    /* Note that connection to OsdWindow::raise above is important. Otherwise OSD window ends up below all other
     * windows after changing main display in Win10. */

    connect(m_window.get(), &QWindow::screenChanged, this, &Osd::updatePosition);
}

Osd::~Osd() {}

void Osd::show(int showMs, int fadeMs) {
    m_fsm->start(showMs, fadeMs);
}

void Osd::hide() {
    m_fsm->stop();
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

Skin* Osd::skin() const {
    return m_window->skin();
}

void Osd::setSkin(Skin* skin) {
    m_window->setSkin(skin);

    updatePosition();
}

Setting* Osd::setting() const {
    return m_setting;
}

void Osd::setSetting(Setting* setting) {
    if (m_setting)
        disconnect(m_setting, nullptr, this, nullptr);

    m_setting = setting;

    if (m_setting) {
        connect(m_setting, &Setting::changed, this, [&] {
            m_window->setState(m_setting->state());
        });

        connect(m_setting, &Setting::activated, this, [&] {
            show(500, 500);
        });
    }
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
