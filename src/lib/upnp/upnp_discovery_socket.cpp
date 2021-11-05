#include "upnp_discovery_socket.h"

#include <cassert>
#include <algorithm>

#include <QtCore/QSysInfo>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QNetworkDatagram>

#include <util/debug.h>

static const int UPNP_MULTICAST_PORT = 1900;
static const char* UPNP_MULTICAST_ADDRESS = "239.255.255.250";
static const int UPNP_MAX_DISCOVERY_ATTEMPTS = 16;
static const int UPNP_DISCOVERY_TIMEOUT_MS = 1000;
static const int UPNP_REDISCOVERY_TIMEOUT_MS = 5 * 60 * 1000;

UpnpDiscoverySocket::UpnpDiscoverySocket(const UpnpDiscoveryOptions& options, QObject* parent):
    QObject(parent),
    m_options(options),
    m_socket(new QUdpSocket(this))
{
    m_options.maxWaitTime = std::clamp(m_options.maxWaitTime, 1, 5); /* As required by UPnP standard. */

    connect(m_socket.get(), &QUdpSocket::readyRead, this, &UpnpDiscoverySocket::readPendingDatagrams);

    m_discoveryMessage = bformat("\
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
}

UpnpDiscoverySocket::~UpnpDiscoverySocket() {
    close();
}

bool UpnpDiscoverySocket::bind() {
    assert(!isBound());

    if (!m_socket->bind(m_options.address, m_options.port)) {
        xWarning("Failed to bind UPnP socket: {}", m_socket->errorString());
        return false;
    }

    return true;
}

bool UpnpDiscoverySocket::isBound() const {
    return m_socket->state() == QAbstractSocket::BoundState;
}

void UpnpDiscoverySocket::close() {
    m_socket->close();
}

void UpnpDiscoverySocket::discover() {
    assert(isBound());

    xInfo("Sending UPnP discovery request");

    qint64 result = m_socket->writeDatagram(m_discoveryMessage, QHostAddress(QLatin1String(UPNP_MULTICAST_ADDRESS)), UPNP_MULTICAST_PORT);
    if (result == -1)
        xWarning("Failed to send UPnP datagram: {}", m_socket->errorString());
}

void UpnpDiscoverySocket::readPendingDatagrams() {
    if (!isBound())
        return;

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

            QByteArray key = line.left(pos).toLower();
            QByteArray value = line.mid(pos + 1).trimmed();

            if (key == QByteArrayLiteral("st")) {
                reply.searchTarget = UpnpSearchTarget(value);
            } else {
                reply.headers.emplace(std::move(key), std::move(value));
            }
        }

        emit discovered(reply);
    }
}
