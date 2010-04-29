#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H
#include <QThread>


#include "AbstractBlobServer.h"
#include <QtNetwork>

/**
 * @file PelicanTCPBlobServer.h
 */

namespace pelican {
    class DataManager;

/**
 * @class PelicanTCPBlobServer
 *  
 * @brief
 *   Threade server that sends the same data from the queue to all connected peers
 * @details
 * 
 */

class PelicanTCPBlobServer : public AbstractBlobServer
{
    Q_OBJECT

    public:
        PelicanTCPBlobServer( QObject* parent = 0 );
        ~PelicanTCPBlobServer();

    protected:
        virtual void run();

    private:
        QTcpServer*       _tcpServer;

    private slots:
        void acceptClientConnection();

        friend class PelicanTCPBlobServerTest;
        
};

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H 
