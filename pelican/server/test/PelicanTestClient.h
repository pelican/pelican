#ifndef PELICANTESTCLIENT_H
#define PELICANTESTCLIENT_H

/**
 * @file PelicanTestClient.h
 */

#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

namespace pelican {
namespace test {

/**
 * @ingroup t_server
 *
 * @class PelicanTestClient
 *
 * @brief
 * Class for establishing a connection to the PelicanServer
 * for testing purposes.
 *
 * @details
 * Designed to work with the TestProtocol.
 */

class PelicanTestClient
{
    public:
        PelicanTestClient( unsigned long port, const QString& host="127.0.0.1" );
        ~PelicanTestClient();
        QString processAcknowledgement() const;
        QString processStreamData() const;

    private:
        quint16 _port;
        QString _host;
        QTcpSocket* _socket;
};

} // namespace test
} // namespace pelican
#endif // PELICANTESTCLIENT_H
