#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H

#include "TCPConnectionManager.h"
#include "AbstractBlobServer.h"
#include <QtNetwork>
#include <QList>
#include <QMap>

/**
 * @file PelicanTCPBlobServer.h
 */

namespace pelican {
    class DataManager;
    class ConfigNode;

/**
 * @class PelicanTCPBlobServer
 *  
 * @brief
 *   Thread server that sends the same data from the queue to all connected peers
 * @details
 * 
 */

class PelicanTCPBlobServer : public QObject, public AbstractBlobServer
{
    Q_OBJECT

    public:
        PelicanTCPBlobServer( const ConfigNode& config, QObject* parent = 0 );
        ~PelicanTCPBlobServer();

    public:
        virtual void send(const QString& streamName, const QByteArray&);

    private:
        TCPConnectionManager*              _connectionManager;
        QMap<QString, QList<QTcpSocket*>>* _clients;

    private:
        friend class PelicanTCPBlobServerTest;
        
};

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H 
