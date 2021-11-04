#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>

#include "upnp_discovery_reply.h"
#include "upnp_search_target.h"

class QUdpSocket;

struct UpnpDiscoveryOptions {
    /** Search target for upnp discovery. */
    UpnpSearchTarget searchTarget = UpnpSearchTarget::all();

    /** Friendly name of the control point, passed in "CPFN.UPNP.ORG" field. */
    QByteArray friendlyName = QByteArrayLiteral("Qnob OSD Tool");

    /** Maximum wait time in seconds that an upnp device should wait before replying to a search request. */
    int maxWaitTime = 1;

    /** Product name, passed in "USER-AGENT". */
    QByteArray productName = QByteArrayLiteral("Qnob");

    /** Product version, passed in "USER-AGENT". */
    QByteArray productVersion = QByteArrayLiteral(QNOB_VERSION_STRING);

    /** Host address to bind to. */
    QHostAddress address = QHostAddress::Any;

    /** Port to bind to. */
    qint16 port = 31337;
};

/**
 * See docs at https://openconnectivity.org/upnp-specs/UPnP-arch-DeviceArchitecture-v2.0-20200417.pdf.
 */
class UpnpDiscovery : public QObject {
    Q_OBJECT
public:
    UpnpDiscovery(const UpnpDiscoveryOptions& options);
    virtual ~UpnpDiscovery();

    const UpnpDiscoveryOptions& options() const {
        return m_options;
    }

    /**
     * Sends a multicast search request.
     */
    void discoverAll();

signals:
    void discovered(UpnpDiscoveryReply reply);

private:
    void readPendingDatagrams();

private:
    UpnpDiscoveryOptions m_options;
    QByteArray m_discoverMessage;
    std::unique_ptr<QUdpSocket> m_socket;
};
