#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QHashFunctions>

class UpnpSearchTarget {
public:
    UpnpSearchTarget();

    static UpnpSearchTarget all();
    static UpnpSearchTarget rootDevices();
    static UpnpSearchTarget device(QByteArrayView hyphenDomainName, QByteArrayView deviceType, QByteArrayView version);

    const QByteArray& toByteArray() const {
        return m_data;
    }

    friend size_t qHash(const UpnpSearchTarget& value) {
        return qHash(value.m_data);
    }

    friend bool operator==(const UpnpSearchTarget& l, const UpnpSearchTarget& r) = default;

private:
    friend class UpnpDiscoveryEndpoint;

    UpnpSearchTarget(const QByteArray& data);

private:
    QByteArray m_data;
};
