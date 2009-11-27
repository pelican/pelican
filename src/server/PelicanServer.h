#ifndef PELICANSERVER_H
#define PELICANSERVER_H

#include <QTcpServer>
#include "utility/memCheck.h"

/**
 * @file PelicanServer.h
 */

namespace pelican {

/**
 * @class PelicanServer
 *  
 * @brief
 * The main data server class to spawn out Sessions
 *
 * @details
 * Initialises the server according to configuration options etc
 * and listens on the specified socket. On an connection a Session
 * object is spawned in another thread
 */

class PelicanServer : public QTcpServer
{
    Q_OBJECT

    public:
        PelicanServer(QObject* parent=0);
        ~PelicanServer();
        void start();

    protected:
        void incomingConnection(int socketDescriptor);

    private:
};

} // namespace pelican
#endif // PELICANSERVER_H 
