#ifndef PELICANTESTCLIENT_H
#define PELICANTESTCLIENT_H

#include <QtCore/QString>
#include <QtNetwork/QTcpSocket>

/**
 * @file PelicanTestClient.h
 */

namespace pelican {

/**
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

} // namespace pelican

#endif // PELICANTESTCLIENT_H 
