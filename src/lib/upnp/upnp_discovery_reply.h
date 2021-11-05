#pragma once

#include <unordered_map>

#include <QtCore/QByteArray>
#include <QtNetwork/QHostAddress>

#include "upnp_search_target.h"

struct UpnpDiscoveryReply {
    /** Source host address. */
    QHostAddress sourceAddress;

    /** Source port. */
    quint16 sourcePort;

    /** UPnP search target, "ST" field. */
    UpnpSearchTarget searchTarget;

    /** UPnP headers that were not parsed. Key is always lowercase. */
    std::unordered_map<QByteArray, QByteArray> headers;
};
