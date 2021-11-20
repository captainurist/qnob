#include "tray_icon.h"

#include <QtGui/QPainter>
#include <QtWidgets/QSystemTrayIcon>

#include <core/skin/skin.h>
#include <core/setting/setting.h>
#include <core/entity/entity_creation_context.h>

#include <platform/platform.h>
#include <platform/platform_metrics.h>

#include "fixed_size_icon_engine.h"

TrayIcon::TrayIcon(QObject* parent) :
    Entity(parent),
    m_trayIcon(new QSystemTrayIcon())
{
    platform()->wheelEventManager()->registerTrayIcon(m_trayIcon.get());

    connect(platform()->metrics(), &PlatformMetrics::trayIconSizeChanged, this, &TrayIcon::updateIcon);
}

TrayIcon::~TrayIcon() {
    platform()->wheelEventManager()->unregisterTrayIcon(m_trayIcon.get());
}

void TrayIcon::loadFromConfig(const EntityCreationContext& ctx) {
    setSkin(ctx.require<Skin*>(lit("skin")));
    setSetting(ctx.require<Setting*>(lit("target")));
}

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

QObject* TrayIcon::icon() const {
    return m_trayIcon.get();
}

void TrayIcon::updateIcon() {
    if (!m_skin || !m_setting || !m_setting->isInitialized()) {
        m_trayIcon->hide();
        return;
    }

    QPixmap pixmap(platform()->metrics()->trayIconSize());
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    m_skin->paint(&painter, pixmap.size(), m_setting->state());
    painter.end();

    /* We need to jump through hoops here with FixedSizeIconEngine to bypass all sizing logic in Qt and use
     * icon sizes that we got above. */
    m_trayIcon->setIcon(QIcon(new FixedSizeIconEngine(pixmap)));
    m_trayIcon->show();
}
