#include "PelicanTestClient.h"
#include <QtNetwork>
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {


// class PelicanTestClient
PelicanTestClient::PelicanTestClient( unsigned long port, const QString& host )
    : _port(port), _host(host)
{
}

PelicanTestClient::~PelicanTestClient()
{
}

/**
 * @details
 * Reads an acknowledgement from the server.
 */
QString PelicanTestClient::processAcknowledgement() const
{
    int timeout = 5000;

    // send request
    QTcpSocket socket;
    socket.connectToHost(_host,_port);

    if (!socket.waitForConnected(timeout)) {
        throw QString(socket.errorString());
    }

    std::cout << "PelicanTestClient: Connected to " << _host.toStdString()
              << ":" << _port << std::endl;
    while (socket.bytesAvailable() < (int)sizeof(quint16)) {
        if (!socket.waitForReadyRead(timeout)) {
            throw QString(socket.errorString());
        }
    }
    std::cout << "PelicanTestClient: Ready to read " << _host.toStdString()
              << ":" << _port << std::endl;

    QByteArray ack;
    ack = socket.readAll();
    std::cout << "PelicanTestClient: Got string ("
              << _host.toStdString() << ":" << _port << ") = "
              << ack.data() <<std::endl;

    return QString("");
}

} // namespace pelican
