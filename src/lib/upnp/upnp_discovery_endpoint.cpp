#include "upnp_discovery_endpoint.h"

#include <cassert>
#include <algorithm>

#include <QtCore/QSysInfo>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/QNetworkInterface>

#include <util/debug.h>

static const int UPNP_MULTICAST_PORT = 1900;
static const char* UPNP_MULTICAST_ADDRESS = "239.255.255.250";

UpnpDiscoveryEndpoint::UpnpDiscoveryEndpoint(QObject* parent):
    QObject(parent)
{}

UpnpDiscoveryEndpoint::~UpnpDiscoveryEndpoint() {
    stop();
}

void UpnpDiscoveryEndpoint::start(QHostAddress address) {
    m_sockets.clear();

    /* Note that it's important to have one socket per network interface because otherwise send tend to fail 
     * randomly with WSAEHOSTUNREACH. Likely trying to send the request through a disconnected vpn adapter. */

    for (QHostAddress address : QNetworkInterface::allAddresses()) {
        if (address.isLoopback())
            continue; /* No reason to bind to loopback. */

        std::unique_ptr<QUdpSocket> socket = std::make_unique<QUdpSocket>(this);
        if (!socket->bind(address)) {
            xWarning("Failed to bind UPnP discovery socket to {}: {}", address.toString(), socket->errorString());
            continue;
        }

        connect(socket.get(), &QUdpSocket::readyRead, this, [this] { readPendingDatagrams(static_cast<QUdpSocket*>(sender())); });

        m_sockets.emplace_back(std::move(socket));
    }

    m_started = true;
}

void UpnpDiscoveryEndpoint::stop() {
    m_sockets.clear();
    m_started = false;
}

void UpnpDiscoveryEndpoint::discover(const UpnpDiscoveryRequest& request) {
    assert(m_started);

    xInfo("Sending UPnP discovery request");

    QByteArray discoveryPacket = bformat("\
M-SEARCH * HTTP/1.1\r\n\
HOST: {}:{}\r\n\
MAN: \"ssdp:discover\"\r\n\
MX: {}\r\n\
ST: {}\r\n\
USER-AGENT: {}/{} UPnP/2.0 {}/{}\r\n\
CPFN.UPNP.ORG: {}\r\n\
\r\n",
        UPNP_MULTICAST_ADDRESS,
        UPNP_MULTICAST_PORT,
        std::clamp(request.maxWaitTime, 1, 5), /* As required by UPnP standard. */
        request.searchTarget.toByteArray(),
        QSysInfo::productType().toUtf8(),
        QSysInfo::productVersion().toUtf8(),
        request.productName,
        request.productVersion,
        request.friendlyName
    );

    bool hasSuccess = false;
    for (auto&& socket : m_sockets) {
        qint64 result = socket->writeDatagram(discoveryPacket, QHostAddress(QLatin1String(UPNP_MULTICAST_ADDRESS)), UPNP_MULTICAST_PORT);
        if (result != -1)
            hasSuccess = true;
    }

    if (!hasSuccess)
        xWarning("Failed to send UPnP datagram");
}

void UpnpDiscoveryEndpoint::readPendingDatagrams(QUdpSocket* socket) {
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();

        UpnpDiscoveryReply message;
        message.sourceAddress = datagram.senderAddress();
        message.sourcePort = datagram.senderPort();

        QByteArrayList lines = datagram.data().split('\n');
        for (QByteArray& line : lines)
            if (line.endsWith('\r'))
                line.chop(1);

        if (lines.size() == 0 || lines[0] != "HTTP/1.1 200 OK") {
            xWarning("Invalid UPnP packet from {}:{}:\n{}", datagram.senderAddress().toString(), datagram.senderPort(), datagram.data());
            continue; /* Skip this datagram altogether. */
        }

        for(size_t i = 1; i < lines.size(); i++) {
            QByteArray& line = lines[i];
            if (line.isEmpty())
                continue;

            qsizetype pos = line.indexOf(':');
            if (pos == -1) {
                xWarning("Invalid line in UPnP packet from {}:{}:\n{}", datagram.senderAddress().toString(), datagram.senderPort(), line);
                continue; /* Just skip this line. */
            }

            QByteArray key = line.left(pos).toLower();
            QByteArray value = line.mid(pos + 1).trimmed();

            if (key == QByteArrayLiteral("st")) {
                message.searchTarget = UpnpSearchTarget(value);
            } else {
                message.headers.emplace(std::move(key), std::move(value));
            }
        }

        emit discovered(message);
    }
}
