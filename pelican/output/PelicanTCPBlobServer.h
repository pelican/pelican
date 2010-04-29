#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H
#include <QThread>


#include "AbstractBlobServer.h"
#include <QtNetwork>
#include <QThread>

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
 *   Threade server that sends the same data from the queue to all connected peers
 * @details
 * 
 */

class PelicanTCPBlobServer : public QThread
{
    Q_OBJECT

    public:
        PelicanTCPBlobServer( const ConfigNode& config, QObject* parent = 0 );
        ~PelicanTCPBlobServer();

    protected:
        virtual void run();

    private:
        QTcpServer* _tcpServer;
        quint16 _port;

    private slots:
        void acceptClientConnection();

        friend class PelicanTCPBlobServerTest;
        
};

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H 
