#pragma once

#include <unordered_map>

#include <QtCore/QByteArray>
#include <QtNetwork/QHostAddress>

struct UpnpDiscoveryReply {
    /** Source host address. */
    QHostAddress sourceAddress;

    /** Source port. */
    quint16 sourcePort;

    /** Upnp headers that were not parsed. Key is always lowercase. */
    std::unordered_map<QByteArray, QByteArray> headers;
};
