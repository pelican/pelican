#ifndef PELICANTESTCLIENT_H
#define PELICANTESTCLIENT_H
#include <QString>

/**
 * @file PelicanTestClient.h
 */

namespace pelican {

/**
 * @class PelicanTestClient
 *  
 * @brief
 *    Class for establishing a connection to the PelicanServer
 *    for testing purposes
 * @details
 *    Designed to work with the TestProtocol
 */

class PelicanTestClient
{
    public:
        PelicanTestClient( unsigned long port, const QString& host="127.0.0.1" );
        ~PelicanTestClient();
        QString request() const;

    private:
        unsigned long _port;
        QString _host;
};

} // namespace pelican
#endif // PELICANTESTCLIENT_H 
