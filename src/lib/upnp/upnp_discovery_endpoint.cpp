#include "upnp_discovery_endpoint.h"

#include <cassert>
#include <algorithm>

#include <QtCore/QSysInfo>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>

#include <util/debug.h>

static const int UPNP_MULTICAST_PORT = 1900;
static const char* UPNP_MULTICAST_ADDRESS = "239.255.255.250";

UpnpDiscoveryEndpoint::UpnpDiscoveryEndpoint(QObject* parent):
    QObject(parent)
{}

UpnpDiscoveryEndpoint::~UpnpDiscoveryEndpoint() {
    stop();
}

void UpnpDiscoveryEndpoint::start(QHostAddress interface) {
    m_passiveSocket.reset(new QUdpSocket(this));
    m_activeSocket.reset(new QUdpSocket(this));

    connect(m_passiveSocket.get(), &QUdpSocket::readyRead, this, [this] {
        readPendingDatagrams(m_passiveSocket.get(), UpnpDiscoveryMessage::NewDeviceNotification);
    });
    connect(m_activeSocket.get(), &QUdpSocket::readyRead, this, [this] {
        readPendingDatagrams(m_activeSocket.get(), UpnpDiscoveryMessage::DiscoveryReply);
    });

    if (!m_passiveSocket->bind(interface, UPNP_MULTICAST_PORT, QUdpSocket::ReuseAddressHint | QUdpSocket::ShareAddress)) {
        xWarning("Failed to bind UPnP socket: {}", m_passiveSocket->errorString());
        return;
    }

    if (!m_activeSocket->bind(interface)) {
        xWarning("Failed to bind UPnP socket: {}", m_activeSocket->errorString());
        return;
    }
}

void UpnpDiscoveryEndpoint::stop() {
    m_passiveSocket.reset();
    m_activeSocket.reset();
}

void UpnpDiscoveryEndpoint::discover(const UpnpDiscoveryRequest& request) {
    assert(m_activeSocket);

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

    qint64 result = m_activeSocket->writeDatagram(discoveryPacket, QHostAddress(QLatin1String(UPNP_MULTICAST_ADDRESS)), UPNP_MULTICAST_PORT);
    if (result == -1)
        xWarning("Failed to send UPnP datagram: {}", m_activeSocket->errorString());
}

void UpnpDiscoveryEndpoint::readPendingDatagrams(QUdpSocket* socket, UpnpDiscoveryMessage::Type type) {
    while (socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = socket->receiveDatagram();

        UpnpDiscoveryMessage message;
        message.type = type;
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
