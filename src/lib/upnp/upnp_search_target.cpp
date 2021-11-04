#include "upnp_search_target.h"

UpnpSearchTarget::UpnpSearchTarget() {}

UpnpSearchTarget::UpnpSearchTarget(const QByteArray& string) :
    m_string(string)
{}

UpnpSearchTarget UpnpSearchTarget::all() {
    return UpnpSearchTarget(QByteArrayLiteral("ssdp:all"));
}

UpnpSearchTarget UpnpSearchTarget::rootDevices() {
    return UpnpSearchTarget(QByteArrayLiteral("upnp:rootdevice"));
}
