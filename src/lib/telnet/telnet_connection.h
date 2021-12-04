#pragma once

#include <memory>
#include <deque>

#include <QtCore/QObject>

#include <util/async/future.h>
#include <util/async/promise.h>

class QTcpSocket;
class QHostAddress;

class TelnetConnection : public QObject {
    Q_OBJECT
public:
    TelnetConnection(QObject* parent);
    virtual ~TelnetConnection();

    Future<void> start(const QHostAddress& address, qint16 port);

    void stop();

    Future<QByteArray> sendCommand(QByteArrayView command);

private:
    void handleSocketConnected();
    void handleSocketErrorOccured();
    void handleSocketBytesAvailable();

private:
    std::unique_ptr<QTcpSocket> m_socket;
    std::unique_ptr<Promise<void>> m_connectPromise;
    std::deque<Promise<QByteArray>> m_commandPromiseQueue;
};
