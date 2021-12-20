#pragma once

#include <memory>
#include <deque>

#include <QtCore/QObject>

#include <util/async/future.h>
#include <util/async/promise.h>

class QHostAddress;
class QTcpSocket;

/**
 * Basically an abstraction around a TCP connection that provides a convenient request-reply API. 
 *
 * Implementation treats CRLF as message separator, but this can be changed.
 */
class StreamingConnection : public QObject {
    Q_OBJECT
public:
    StreamingConnection(QObject* parent);
    virtual ~StreamingConnection();

    Future<void> start(const QHostAddress& address, qint16 port);

    void stop();

    Future<QByteArray> send(const QByteArray& request);

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
