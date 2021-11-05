#pragma once

#include <memory>

#include <QtCore/QObject>

#include "upnp_discovery_reply.h"
#include "upnp_discovery_options.h"

class QUdpSocket;

/**
 * A thin wrapper around a UDP socket for UPnP discovery.
 *
 * See docs at https://openconnectivity.org/upnp-specs/UPnP-arch-DeviceArchitecture-v2.0-20200417.pdf.
 */
class UpnpDiscoverySocket : public QObject {
    Q_OBJECT
public:
    UpnpDiscoverySocket(const UpnpDiscoveryOptions& options, QObject* parent);
    virtual ~UpnpDiscoverySocket();

    const UpnpDiscoveryOptions& options() const {
        return m_options;
    }

    /**
     * Binds this discovery socket.
     *
     * \returns                         Whether this socket was successfully bound.
     */
    bool bind();

    /**
     * \returns                         Whether this socket is bound.
     */
    bool isBound() const;

    /**
     * Closes this discovery socket.
     */
    void close();

    /**
     * Sends a single discovery request. Results will be returned via `discovered` signal.
     */
    void discover();

signals:
    void discovered(UpnpDiscoveryReply reply);

private:
    void readPendingDatagrams();

private:
    UpnpDiscoveryOptions m_options;
    QByteArray m_discoveryMessage;
    std::unique_ptr<QUdpSocket> m_socket;
};
