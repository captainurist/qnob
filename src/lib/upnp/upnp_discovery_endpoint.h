#pragma once

#include <memory>

#include <QtCore/QObject>

#include "upnp_discovery_reply.h"
#include "upnp_discovery_request.h"

class QUdpSocket;

// TODO: binding to Any results in send failures, gotta bind to each interface separately.
// TODO: also apparently nobody sends ssdp:alive, so there is no reason to listen to it.

/**
 * This class performs active UPnP discovery (via "M-SEARCH" requests).
 * 
 * There is also "passive" discovery that can be done by listening to "NOTIFY" packets that UPnP devices are supposed
 * to send out when they are connected to the network, but apparently neither HEOS nor Hue do this, so this is not 
 * implemented.
 *
 * See docs at https://openconnectivity.org/upnp-specs/UPnP-arch-DeviceArchitecture-v2.0-20200417.pdf.
 */
class UpnpDiscoveryEndpoint : public QObject {
    Q_OBJECT
public:
    UpnpDiscoveryEndpoint(QObject* parent);
    virtual ~UpnpDiscoveryEndpoint();

    // TODO: #exceptions

    /**
     * Starts listening to UPnP discovery messages.
     *
     * \param address                   Address to listen on.
     */
    void start(QHostAddress address = QHostAddress::AnyIPv4);

    /**
     * Stops listening to UPnP discovery messages. Does nothing if already stopped.
     */
    void stop();

    /**
     * Sends a single discovery request. Results will be returned via `discovered` signal.
     * Endpoint must be started before calling this method.
     */
    void discover(const UpnpDiscoveryRequest& request = UpnpDiscoveryRequest());

signals:
    void discovered(const UpnpDiscoveryReply& reply);

private:
    void readPendingDatagrams(QUdpSocket* socket);

private:
    std::vector<std::unique_ptr<QUdpSocket>> m_sockets;
    bool m_started = false;
};
