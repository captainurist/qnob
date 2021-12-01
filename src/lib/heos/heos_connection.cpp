#include "heos_connection.h"

#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpSocket>

#include <util/debug.h>

static const int HEOS_CLI_PORT = 1255;

HeosConnection::HeosConnection(QObject* parent):
    QObject(parent)
{
}

HeosConnection::~HeosConnection() {}

void HeosConnection::start(const QHostAddress& address) {
    m_address = address;
    m_socket.reset(new QTcpSocket(this));

    connect(m_socket.get(), &QTcpSocket::connected, this, &HeosConnection::handleSocketConnected);
    connect(m_socket.get(), &QTcpSocket::errorOccurred, this, &HeosConnection::handleSocketErrorOccured);
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &HeosConnection::handleSocketBytesAvailable);

    m_socket->connectToHost(address, HEOS_CLI_PORT);
}

void HeosConnection::stop() {
    m_socket->reset();
}

QByteArray HeosConnection::sendCommand(const QByteArray& command) {
    while (m_socket->state() != QTcpSocket::ConnectedState)
        qApp->processEvents();

    m_socket->write(command + "\r\n");
    m_socket->flush();

    while(!m_socket->bytesAvailable())
        qApp->processEvents();

    return QByteArray();
}

void HeosConnection::handleSocketConnected() {

}

void HeosConnection::handleSocketErrorOccured() {
    xWarning("Could not connect to HEOS host {}", m_address.toString());
}

void HeosConnection::handleSocketBytesAvailable() {
    xInfo("{}", m_socket->readAll());
}
