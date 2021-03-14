#include "tray_icon.h"

#include <QtGui/QPainter>
#include <QtWidgets/QSystemTrayIcon>

#include <core/skin/skin.h>
#include <core/setting/setting.h>

TrayIcon::TrayIcon(const QString& id) :
    Entity(id),
    m_trayIcon(new QSystemTrayIcon())
{}

TrayIcon::~TrayIcon() {}

Skin* TrayIcon::skin() const {
    return m_skin;
}

void TrayIcon::setSkin(Skin* skin) {
    m_skin = skin;

    updateIcon();
}

Setting* TrayIcon::setting() const {
    return m_setting;
}

void TrayIcon::setSetting(Setting* setting) {
    if (m_setting)
        disconnect(setting, nullptr, this, nullptr);

    m_setting = setting;

    if (m_setting)
        connect(m_setting, &Setting::changed, this, &TrayIcon::updateIcon);

    updateIcon();
}

void TrayIcon::updateIcon() {
    if (!m_skin || !m_setting || !m_setting->isInitialized()) {
        m_trayIcon->hide();
        return;
    }

    QPixmap pixmap(m_skin->size());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    m_skin->paint(&painter, m_setting->state());
    painter.end();

    m_trayIcon->setIcon(pixmap);
    m_trayIcon->show();
}
