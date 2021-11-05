#include "upnp_discovery.h"

#include <cassert>

#include <QtCore/QTimer>

#include "upnp_discovery_socket.h"

static const int UPNP_MAX_DISCOVERY_ATTEMPTS = 16;
static const int UPNP_DISCOVERY_TIMEOUT_MS = 1000;
static const int UPNP_REDISCOVERY_TIMEOUT_MS = 5 * 60 * 1000;

UpnpDiscovery::UpnpDiscovery(const UpnpDiscoveryOptions& options) :
    m_socket(new UpnpDiscoverySocket(options)),
    m_discoveryTimer(new QTimer()),
    m_rediscoveryTimer(new QTimer())
{
    connect(m_socket.get(), &UpnpDiscoverySocket::discovered, this, &UpnpDiscovery::handleDiscovered);
    connect(m_discoveryTimer.get(), &QTimer::timeout, this, &UpnpDiscovery::discoveryTick);
    connect(m_rediscoveryTimer.get(), &QTimer::timeout, this, &UpnpDiscovery::rediscoveryTick);
}

UpnpDiscovery::~UpnpDiscovery() {
    stop();
}

void UpnpDiscovery::start() {
    assert(!isRunning());

    if (!m_socket->bind())
        return;

    m_discoveryTimer->start(UPNP_DISCOVERY_TIMEOUT_MS);
    m_rediscoveryTimer->start(UPNP_REDISCOVERY_TIMEOUT_MS);
    m_discoveryAttemptsLeft = UPNP_MAX_DISCOVERY_ATTEMPTS;

    discoveryTick();
}

void UpnpDiscovery::stop() {
    m_socket->close();
    m_discoveryTimer->stop();
    m_rediscoveryTimer->stop();
}

bool UpnpDiscovery::isRunning() const {
    return m_socket->isBound();
}

void UpnpDiscovery::discoveryTick() {
    m_discoveryAttemptsLeft--;

    if (m_discoveryAttemptsLeft <= 0) {
        m_discoveryTimer->stop();
        return;
    }

    m_socket->discover();
}

void UpnpDiscovery::rediscoveryTick() {
    m_socket->discover();
}

void UpnpDiscovery::handleDiscovered(const UpnpDiscoveryReply& reply) {
    m_discoveryTimer->stop(); /* Don't try again please. */

    emit discovered(reply);
}
