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
 * Some description of what this does?!
 */
QString PelicanTestClient::request() const
{
    int timeout = 500000000;

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

    quint16 blockSize;
    QDataStream in(&socket);
    in.setVersion(QDataStream::Qt_4_0);
    in >> blockSize;
    std::cout << "PelicanTestClient: Got Blocksize ("
              << _host.toStdString() << ":" << _port << ") = "
              << blockSize <<std::endl;

    if (in.atEnd())
        return QString("");

    while (socket.bytesAvailable() < blockSize) {
        if (!socket.waitForReadyRead(timeout)) {
            throw QString("PelicanTestClient: !ERROR! " + socket.errorString());
        }
    }

    QString result;
    in >> result;
    std::cout << "PelicanTestClient: Got result (" << _host.toStdString()
              << ":" << _port << ") = " << result.toStdString() <<std::endl;

    return result;
}

} // namespace pelican
