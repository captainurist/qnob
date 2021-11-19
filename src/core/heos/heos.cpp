#include "heos.h"

#include <lib/upnp/upnp_discovery_endpoint.h>
#include <lib/upnp/upnp_discovery_reply.h>

#include <util/debug.h>

Heos::Heos(UpnpDiscoveryEndpoint* endpoint, QObject* parent):
	Entity(parent)
{
	m_upnpSearchTarget = UpnpSearchTarget::device("schemas-denon-com", "ACT-Denon", "1");

	connect(endpoint, &UpnpDiscoveryEndpoint::discovered, this, &Heos::handleDiscovered);
	endpoint->discover();
}

Heos::~Heos() {}

void Heos::handleDiscovered(const UpnpDiscoveryReply& reply) {
	if (reply.searchTarget != m_upnpSearchTarget)
		return;

	xInfo("Discovered HEOS device at {}", reply.sourceAddress.toString());
}

