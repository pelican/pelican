#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H
#include <QThread>


#include "AbstractBlobServer.h"

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

class PelicanTCPBlobServer : public AbstractBlobServer, public QThread
{
    Q_OBJECT

    public:
        PelicanTCPBlobServer( QObject* parent = 0 );
        ~PelicanTCPBlobServer();
        virtual void send(const QString& type, const QBuffer& incoming);

    protected:
        virtual void run();

    private:
        DataManager* _dataManager;
        
};

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H 
