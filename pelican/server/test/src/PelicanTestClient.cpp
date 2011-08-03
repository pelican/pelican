#include "pelican/server/test/PelicanTestClient.h"

#include <QtNetwork/QTcpSocket>

#include <iostream>

namespace pelican {
namespace test {


// class PelicanTestClient
PelicanTestClient::PelicanTestClient( unsigned long port, const QString& host )
{
    _host = host;
    _port = port;
    _socket = new QTcpSocket;
}

PelicanTestClient::~PelicanTestClient()
{
    _socket->abort();
    delete _socket;
}

/**
 * @details
 * Reads an acknowledgement from the server.
 */
QString PelicanTestClient::processAcknowledgement() const
{
    int timeout = 5000;

    // send request
    _socket->connectToHost(_host,_port);

    if (!_socket->waitForConnected(timeout)) {
        throw QString("PelicanTestClient: Wait for connected: " +
                _socket->errorString());
    }

    std::cout << "PelicanTestClient: Connected to " << _host.toStdString()
              << ":" << _port << std::endl;
    while (_socket->bytesAvailable() < (int)sizeof(quint16)) {
        if (!_socket->waitForReadyRead(timeout)) {
            throw QString("PelicanTestClient: Wait for ready read: " +
                    _socket->errorString());
        }
    }
    std::cout << "PelicanTestClient: Ready to read " << _host.toStdString()
              << ":" << _port << std::endl;

    QByteArray ack;
    ack = _socket->readAll();
    std::cout << "PelicanTestClient: Got string ("
              << _host.toStdString() << ":" << _port << ") = "
              << ack.data() <<std::endl;

    return QString("");
}

/**
 * @details
 * Reads an acknowledgement from the server.
 */
QString PelicanTestClient::processStreamData() const
{
    int timeout = 5000;

    // send request
    _socket->connectToHost(_host,_port);

    if (!_socket->waitForConnected(timeout)) {
        throw QString("PelicanTestClient: Wait for connected: " +
                _socket->errorString());
    }

    std::cout << "PelicanTestClient::processStreamData(): Connected to " << _host.toStdString()
              << ":" << _port << std::endl;
    while (_socket->bytesAvailable() < (int)sizeof(quint16)) {
        if (!_socket->waitForReadyRead(timeout)) {
            throw QString("PelicanTestClient: Wait for ready read: " +
                    _socket->errorString());
        }
    }
    std::cout << "PelicanTestClient::processStreamData(): Ready to read " << _host.toStdString()
              << ":" << _port << std::endl;

    QByteArray ack;
    ack = _socket->readAll();
    std::cout << "PelicanTestClient::processStreamData(): Got string ("
              << _host.toStdString() << ":" << _port << ") = "
              << ack.data() <<std::endl;

    return QString("");
}

} // namespace test
} // namespace pelican
