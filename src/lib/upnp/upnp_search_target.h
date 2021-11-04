#pragma once

#include <QtCore/QByteArray>

class UpnpSearchTarget {
public:
    UpnpSearchTarget();

    static UpnpSearchTarget all();
    static UpnpSearchTarget rootDevices();
    //static QByteArray device(const QByteArray& hyphenDomainName, const QByteArray& serviceType, )

    const QByteArray& string() const {
        return m_string;
    }

private:
    UpnpSearchTarget(const QByteArray& string);

private:
    QByteArray m_string;
};
