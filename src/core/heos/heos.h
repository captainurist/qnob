#pragma once

#include <core/entity/entity.h>

#include <lib/upnp/upnp_search_target.h>

class UpnpDiscoveryEndpoint;
class UpnpDiscoveryReply;

class Heos : public Entity {
    Q_OBJECT
public:
    Heos(const QString& id, UpnpDiscoveryEndpoint* endpoint);
    virtual ~Heos();

private:
    void handleDiscovered(const UpnpDiscoveryReply& reply);

private:
    UpnpSearchTarget m_upnpSearchTarget;
};
