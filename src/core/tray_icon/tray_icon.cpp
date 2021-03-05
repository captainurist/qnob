#include "tray_icon.h"

#include <QtGui/QPainter>
#include <QtWidgets/QSystemTrayIcon>

#include <core/skin/skin.h>

TrayIcon::TrayIcon(const QString& id) :
    Entity(id),
    m_trayIcon(new QSystemTrayIcon())
{
    m_trayIcon->show();
}

TrayIcon::~TrayIcon() {}

Skin* TrayIcon::skin() const {
    return m_skin;
}

void TrayIcon::setSkin(Skin* skin) {
    m_skin = skin;

    updateIcon();
}

KnobState TrayIcon::state() const {
    return m_state;
}

void TrayIcon::setState(const KnobState& state) {
    if (state == m_state)
        return;

    m_state = state;

    updateIcon();
}

void TrayIcon::updateIcon() {
    if (!m_skin) {
        m_trayIcon->setIcon(QIcon());
        return;
    }

    QPixmap pixmap(m_skin->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    m_skin->paint(&painter, m_state);
    painter.end();

    m_trayIcon->setIcon(pixmap);
}
