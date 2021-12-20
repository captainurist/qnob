#include "streaming_connection.h"

#include <cassert>

#include <QtNetwork/QTcpSocket>

#include <util/exception/cancelled_exception.h>

StreamingConnection::StreamingConnection(QObject* parent) :
    QObject(parent) 
{}

StreamingConnection::~StreamingConnection() {}

Future<void> StreamingConnection::start(const QHostAddress& address, qint16 port) {
    stop();

    m_socket = std::make_unique<QTcpSocket>(this);
    m_connectPromise = std::make_unique<Promise<void>>();
    
    connect(m_socket.get(), &QTcpSocket::connected, this, &StreamingConnection::handleSocketConnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &StreamingConnection::handleSocketErrorOccured);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &StreamingConnection::handleSocketBytesAvailable);

    m_socket->connectToHost(address, port);

    return m_connectPromise->future();
}

void StreamingConnection::stop() {
    m_socket.reset();

    if (m_connectPromise) {
        m_connectPromise->set_error(std::make_exception_ptr(CancelledException()));
        m_connectPromise.reset();
    }

    for (Command& command : m_commandQueue)
        command.promise.set_error(std::make_exception_ptr(CancelledException()));
    m_commandQueue.clear();
}

Future<QByteArray> StreamingConnection::send(const QByteArray& payload) {
    assert(m_socket); /* Must call start() first. */

    Command& command = m_commandQueue.emplace_back();
    command.payload = payload;

    // TODO: We're not handling 0xff bytes properly here. Gotta duplicate them.

    switch (m_socket->state()) {
    case QAbstractSocket::UnconnectedState:
        /* Error has occurred. */
        command.promise.set_error(std::make_exception_ptr(Exception(lit("error")))); // TODO
        return command.promise.future();

    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        /* Gotta wait before we can send the data. */
        return command.promise.future();

    case QAbstractSocket::ConnectedState:
        flushCommand(payload);
        return command.promise.future();

    case QAbstractSocket::BoundState:
    case QAbstractSocket::ListeningState:
    case QAbstractSocket::ClosingState:
    default:
        /* We should never get here. */
        assert(false);
        return Future<QByteArray>();
    }
}

void StreamingConnection::handleSocketConnected() {
    assert(m_connectPromise);

    m_connectPromise->set_value();
    m_connectPromise.reset();

    for (Command& command : m_commandQueue)
        flushCommand(command.payload);
}

void StreamingConnection::handleSocketErrorOccured() {
    if (m_connectPromise) {
        m_connectPromise->set_error(std::make_exception_ptr(Exception(lit("error")))); // TODO: IoException
        m_connectPromise.reset();
    }

    for (Command& command : m_commandQueue)
        command.promise.set_error(std::make_exception_ptr(Exception(lit("error")))); // TODO: IoException
    m_commandQueue.clear();
}

void StreamingConnection::handleSocketBytesAvailable() {
    if (!m_socket->canReadLine())
        return; /* I bet this is not super efficient */
}

void StreamingConnection::flushCommand(const QByteArray& payload) {
    m_socket->write(payload);
    m_socket->flush();
}
