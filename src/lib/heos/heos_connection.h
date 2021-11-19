#pragma once

#include <memory>

#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>

class QTcpSocket;

/**
 * Connection to a HEOS device.
 *
 * See v1.13 spec at https://rn.dmglobal.com/euheos/HEOS_CLI_ProtocolSpecification.pdf.
 */
class HeosConnection: public QObject {
    Q_OBJECT
public:
    HeosConnection(QObject* parent);
    virtual ~HeosConnection();

    void start(const QHostAddress& address);

    void stop();

    QByteArray sendCommand(const QByteArray& command);

private:
    void handleSocketConnected();
    void handleSocketErrorOccured();
    void handleSocketBytesAvailable();

private:
    std::unique_ptr<QTcpSocket> m_socket;
    QHostAddress m_address;
};

