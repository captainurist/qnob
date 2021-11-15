#pragma once

#include <memory>

#include <QtCore/QObject>

#include "upnp_discovery_message.h"
#include "upnp_discovery_request.h"

class QUdpSocket;

// TODO: binding to Any results in send failures, gotta bind to each interface separately.
// TODO: also apparently nobody sends ssdp:alive, so there is no reason to listen to it.

/**
 * UPnP discovery endpoint that supports both active (via "M-SEARCH" requests) and passive (listening on port 1900)
 * discovery.
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
    void discovered(UpnpDiscoveryMessage message);

private:
    void readPendingDatagrams(QUdpSocket* socket, UpnpDiscoveryMessage::Type type);

private:
    std::unique_ptr<QUdpSocket> m_passiveSocket;
    std::unique_ptr<QUdpSocket> m_activeSocket;
};
