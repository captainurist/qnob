#pragma once

#include <memory>
#include <deque>

#include <QtCore/QObject>

#include <util/async/future.h>
#include <util/async/promise.h>

class QTcpSocket;
class QHostAddress;

// TODO: nah, this doesn't work. This is not a telnel client. Telnet is not a request-reply protocol, there can be
// prompts on connection. At its core it's just a streaming protocol with control chars on top.
// SO this is then a chunked tcp connection, not a telnet connection =(.

// TODO: don't send next command before receiving a full reply to the previous one?

// handler:
//    void reset();
//    QVariant handleChunk(QByteArrayView chunk); -> can throw!
//    QVariant handleTimeout(); -> can throw!
//

class TelnetConnection : public QObject {
    Q_OBJECT
public:
    TelnetConnection(QObject* parent);
    virtual ~TelnetConnection();

    Future<void> start(const QHostAddress& address, qint16 port);

    void stop();

    Future<QByteArray> sendCommand(const QByteArray& payload);

private:
    void handleSocketConnected();
    void handleSocketErrorOccured();
    void handleSocketBytesAvailable();

    void flushCommand(const QByteArray& payload);

private:
    struct Command {
        Promise<QByteArray> promise;
        QByteArray payload;
    };

    std::unique_ptr<QTcpSocket> m_socket;
    std::unique_ptr<Promise<void>> m_connectPromise;
    std::deque<Command> m_commandQueue;
};
