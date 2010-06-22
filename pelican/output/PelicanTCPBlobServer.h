#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H

#include "pelican/output/TCPConnectionManager.h"
#include "pelican/output/AbstractBlobServer.h"

#include <QtCore/QList>
#include <QtCore/QMap>

/**
 * @file PelicanTCPBlobServer.h
 */

namespace pelican {

class DataManager;
class ConfigNode;
class DataBlob;
class ThreadedBlobServer;

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
        quint16 serverPort() const;

    public:
        virtual void send(const QString& streamName, const DataBlob* dataBlob);

    private:
        ThreadedBlobServer* _server;
        TCPConnectionManager*  _connectionManager;

    private:
        friend class PelicanTCPBlobServerTest;

};

PELICAN_DECLARE_MODULE(PelicanTCPBlobServer)

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H
