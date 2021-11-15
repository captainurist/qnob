#pragma once

#include <core/entity/entity.h>

#include <lib/upnp/upnp_search_target.h>

class UpnpDiscoveryEndpoint;
class UpnpDiscoveryMessage;

class Heos : public Entity {
    Q_OBJECT
public:
    Heos(const QString& id, UpnpDiscoveryEndpoint* endpoint);
    virtual ~Heos();

private:
    void handleDiscovered(const UpnpDiscoveryMessage& reply);

private:
    UpnpSearchTarget m_upnpSearchTarget;
};
