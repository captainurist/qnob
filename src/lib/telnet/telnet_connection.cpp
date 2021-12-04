#include "telnet_connection.h"

#include <cassert>

#include <QtNetwork/QTcpSocket>

#include <util/exception/cancelled_exception.h>

TelnetConnection::TelnetConnection(QObject* parent):
    QObject(parent)
{}

TelnetConnection::~TelnetConnection() {}

Future<void> TelnetConnection::start(const QHostAddress& address, qint16 port) {
    // TODO: cleanup old state!

    m_socket = std::make_unique<QTcpSocket>(this);
    m_connectPromise = std::make_unique<Promise<void>>();

    connect(m_socket.get(), &QTcpSocket::connected, this, &TelnetConnection::handleSocketConnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &TelnetConnection::handleSocketErrorOccured);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &TelnetConnection::handleSocketBytesAvailable);

    m_socket->connectToHost(address, port);

    return m_connectPromise->future();
}

void TelnetConnection::stop() {
    m_socket.reset();

    if (m_connectPromise) {
        m_connectPromise->set_error(std::make_exception_ptr(CancelledException()));
        m_connectPromise.reset();
    }
}

Future<QByteArray> TelnetConnection::sendCommand(QByteArrayView command) {
    assert(m_socket); /* Must call start() first. */

    Promise<QByteArray> promise;

    switch (m_socket->state()) {
    case QAbstractSocket::UnconnectedState:
        /* Error has occurred. */
        //promise.setException(std::make_exception_ptr(Exception(lit("error"))));
        return promise.future();

    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
        /* Gotta wait before we can send the data. */

    case QAbstractSocket::ConnectedState:

    case QAbstractSocket::BoundState:
    case QAbstractSocket::ListeningState:
    case QAbstractSocket::ClosingState:
    default:
        /* We should never get here. */
        assert(false);
        return Future<QByteArray>();
    }

}

void TelnetConnection::handleSocketConnected() {
    assert(m_connectPromise && m_connectPromise->valid());

    m_connectPromise->set_value();
    m_connectPromise.reset();
}

void TelnetConnection::handleSocketErrorOccured() {
    if (m_connectPromise) {
        //m_connectPromise->(std::make_exception_ptr(Exception(lit("error"))));

    }

    //m_connectPromise.reset();
    //m_socket.reset();
}

void TelnetConnection::handleSocketBytesAvailable() {

}
