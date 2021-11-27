#include "telnet_connection.h"

#include <cassert>

#include <QtNetwork/QTcpSocket>

#include <util/exception/exception.h>

//#include "promise.h"

TelnetConnection::TelnetConnection(QObject* parent):
    QObject(parent)
{}

TelnetConnection::~TelnetConnection() {}

#if 0
Future<void> TelnetConnection::start(const QHostAddress& address, qint16 port) {
    // TODO: cleanup old state!

    m_socket = std::make_unique<QTcpSocket>(this);
    //m_connectPromise = std::make_unique<Promise<void>>();

    connect(m_socket.get(), &QTcpSocket::connected, this, &TelnetConnection::handleSocketConnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &TelnetConnection::handleSocketErrorOccured);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &TelnetConnection::handleSocketBytesAvailable);

    m_socket->connectToHost(address, port);

    //return m_connectPromise->future();
    return Future<void>();
}
#endif

void TelnetConnection::stop() {
    m_socket.reset();
}

#if 0
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
#endif

void TelnetConnection::handleSocketConnected() {
    //m_connectPromise->setValue();
    //m_connectPromise.reset();
}

void TelnetConnection::handleSocketErrorOccured() {
    //m_connectPromise->setException(std::make_exception_ptr(Exception(lit("error"))));
    //m_connectPromise.reset();
    //m_socket.reset();
}

void TelnetConnection::handleSocketBytesAvailable() {

}
