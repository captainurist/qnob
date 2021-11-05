#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QHashFunctions>

class UpnpSearchTarget {
public:
    UpnpSearchTarget();

    static UpnpSearchTarget all();
    static UpnpSearchTarget rootDevices();
    static UpnpSearchTarget device(QByteArrayView hyphenDomainName, QByteArrayView deviceType, QByteArrayView version);

    const QByteArray& string() const {
        return m_string;
    }

    friend size_t qHash(const UpnpSearchTarget& value) {
        return qHash(value.m_string);
    }

    friend bool operator==(const UpnpSearchTarget& l, const UpnpSearchTarget& r) = default;

private:
    friend class UpnpDiscoverySocket;

    UpnpSearchTarget(const QByteArray& string);

private:
    QByteArray m_string;
};
