#pragma once

#include <QtCore/QByteArray>
#include <QtNetwork/QHostAddress>

#include "upnp_search_target.h"

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

    /** Port to bind to. Zero means random port. */
    qint16 port = 0;
};
