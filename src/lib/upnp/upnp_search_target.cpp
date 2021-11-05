#include "upnp_search_target.h"

#include <util/format.h>

UpnpSearchTarget::UpnpSearchTarget() {}

UpnpSearchTarget::UpnpSearchTarget(const QByteArray& string) :
    m_string(string)
{}

UpnpSearchTarget UpnpSearchTarget::all() {
    return QByteArrayLiteral("ssdp:all");
}

UpnpSearchTarget UpnpSearchTarget::rootDevices() {
    return QByteArrayLiteral("upnp:rootdevice");
}

UpnpSearchTarget UpnpSearchTarget::device(QByteArrayView hyphenDomainName, QByteArrayView deviceType, QByteArrayView version) {
    return bformat("urn:{}:device:{}:{}", hyphenDomainName, deviceType, version);
}
