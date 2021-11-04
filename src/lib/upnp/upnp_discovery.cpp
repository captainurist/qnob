#include "upnp_discovery.h"

#include <cassert>
#include <algorithm>

#include <QtCore/QSysInfo>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>

#include <util/debug.h>

static const int UPNP_MULTICAST_PORT = 1900;
static const char* UPNP_MULTICAST_ADDRESS = "239.255.255.250";

UpnpDiscovery::UpnpDiscovery(const UpnpDiscoveryOptions& options):
    m_options(options),
    m_socket(new QUdpSocket())
{
    if (!m_socket->bind(m_options.address, m_options.port))
        xWarning("Failed to bind UPnP socket: {}", m_socket->errorString());

    connect(m_socket.get(), &QUdpSocket::readyRead, this, &UpnpDiscovery::readPendingDatagrams);
}

UpnpDiscovery::~UpnpDiscovery() {}

void UpnpDiscovery::discoverAll() {
    if (m_socket->state() != QUdpSocket::BoundState)
        return;

    /* See docs at https ://openconnectivity.org/upnp-specs/UPnP-arch-DeviceArchitecture-v2.0-20200417.pdf. */
    QByteArray discoveryMessage = bformat("\
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
        m_options.maxWaitTime,
        m_options.searchTarget.string(),
        QSysInfo::productType().toUtf8(),
        QSysInfo::productVersion().toUtf8(),
        m_options.productName,
        m_options.productVersion,
        m_options.friendlyName
    );

    qint64 result = m_socket->writeDatagram(discoveryMessage, QHostAddress(QLatin1String(UPNP_MULTICAST_ADDRESS)), UPNP_MULTICAST_PORT);
    if (result == -1)
        xWarning("Failed to send UPnP datagram: {}", m_socket->errorString());
}

void UpnpDiscovery::readPendingDatagrams() {
    while (m_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_socket->receiveDatagram();

        UpnpDiscoveryReply reply;
        reply.sourceAddress = datagram.senderAddress();
        reply.sourcePort = datagram.senderPort();

        QByteArrayList lines = datagram.data().split('\n');
        for (QByteArray& line : lines)
            if (line.endsWith('\r'))
                line.chop(1);

        if (lines.size() == 0 || lines[0] != "HTTP/1.1 200 OK") {
            xWarning("Invalid UPnP reply from {}:{}:\n{}", datagram.senderAddress().toString(), datagram.senderPort(), datagram.data());
            continue; /* Skip this datagram altogether. */
        }

        for(size_t i = 1; i < lines.size(); i++) {
            QByteArray& line = lines[i];
            if (line.isEmpty())
                continue;

            qsizetype pos = line.indexOf(':');
            if (pos == -1) {
                xWarning("Invalid line in UPnP reply from {}:{}:\n{}", datagram.senderAddress().toString(), datagram.senderPort(), line);
                continue; /* Just skip this line. */
            }

            reply.headers.emplace(line.left(pos).toLower(), line.right(pos + 1).trimmed());
        }

        //xInfo("{}", datagram.data());

        emit discovered(reply);
    }
}
