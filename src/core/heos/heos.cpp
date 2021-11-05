#include "heos.h"

#include <lib/upnp/upnp_discovery.h>
#include <lib/upnp/upnp_discovery_reply.h>

#include <util/debug.h>

Heos::Heos(const QString& id, UpnpDiscovery* upnpDiscovery):
	Entity(id)
{
	m_upnpSearchTarget = UpnpSearchTarget::device("schemas-denon-com", "ACT-Denon", "1");

	connect(upnpDiscovery, &UpnpDiscovery::discovered, this, &Heos::handleDiscovered);
}

Heos::~Heos() {}

void Heos::handleDiscovered(const UpnpDiscoveryReply& reply) {
	if (reply.searchTarget != m_upnpSearchTarget)
		return;

	xInfo("Discovered HEOS device at {}", reply.sourceAddress);
}

