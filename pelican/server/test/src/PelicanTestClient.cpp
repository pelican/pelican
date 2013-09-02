#include "server/test/PelicanTestClient.h"

#include <QtNetwork/QTcpSocket>
#include <QtCore/QDebug>

#include <iostream>

namespace pelican {
namespace test {


// class PelicanTestClient
PelicanTestClient::PelicanTestClient(unsigned long port, const QString& host,
        bool verbose)
{
    _host = host;
    _port = port;
    _socket = new QTcpSocket;
    _verbose = verbose;
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
    using namespace std;
    int timeout = 5000;

    // send request
    _socket->connectToHost(_host,_port);

    if (!_socket->waitForConnected(timeout))
    {
        throw QString("PelicanTestClient: Wait for connected: " +
                _socket->errorString());
    }

    if (_verbose) {
        cout << "PelicanTestClient: Connected to " << _host.toStdString()
             << ":" << _port << endl;
    }
    while (_socket->bytesAvailable() < (int)sizeof(quint16))
    {
        if (!_socket->waitForReadyRead(timeout))
        {
            throw QString("PelicanTestClient: Wait for ready read: " +
                    _socket->errorString());
        }
    }
    if (_verbose) {
        cout << "PelicanTestClient: Ready to read " << _host.toStdString()
             << ":" << _port << endl;
    }

    QByteArray ack;
    ack = _socket->readAll();
    if (_verbose) {
        cout << "PelicanTestClient: Got string (" << _host.toStdString()
             << ":" << _port << ") = " << ack.data() << endl;
    }

    return QString("");
}

/**
 * @details
 * Reads an acknowledgement from the server.
 */
QString PelicanTestClient::processStreamData() const
{
    using namespace std;
    int timeout = 5000;

    // send request
    _socket->connectToHost(_host, _port);

    if (!_socket->waitForConnected(timeout))
    {
        throw QString("PelicanTestClient: Wait for connected: " +
                _socket->errorString());
    }

    if (_verbose) {
        cout << "PelicanTestClient::processStreamData(): Connected to "
             << _host.toStdString() << ":" << _port << endl;
    }

    while (_socket->bytesAvailable() < (int)sizeof(quint16))
    {
        if (!_socket->waitForReadyRead(timeout))
        {
            throw QString("PelicanTestClient: Wait for ready read: " +
                    _socket->errorString());
        }
    }
    if (_verbose) {
        cout << "PelicanTestClient::processStreamData(): Ready to read "
             << _host.toStdString()
             << ":" << _port << endl;
    }

    QByteArray ack;
    ack = _socket->readAll();
    // The following removes the error code sent by the TestProtocol
    // from the beginning message when sending an error with the
    // TestProtocol::sendError() method.
    if (ack.startsWith(2)) // 2 == TestProtocol::error
    {
        ack.remove(0, sizeof(char));
    }
    if (_verbose) {
        cout << "PelicanTestClient::processStreamData(): Got string ("
             << _host.toStdString() << ":" << _port << ") = "
             << ack.data() << endl;
    }

    return QString();
}

} // namespace test
} // namespace pelican
