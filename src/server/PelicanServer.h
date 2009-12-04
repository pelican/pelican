#ifndef PELICANSERVER_H
#define PELICANSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QThread>

/**
 * @file PelicanServer.h
 */

namespace pelican {

class AbstractProtocol;
class PelicanPortServer;

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

class PelicanServer : public QThread
{
    Q_OBJECT

    public:
        PelicanServer(QObject* parent=0);
        ~PelicanServer();

        /// Assosiate an incoming port with a particular
        //  protocol. Ownership of AbstractProtocol is
        //  transferred to this class.
        void addProtocol(AbstractProtocol*, quint16 port);

    protected:
        // code to be executed in the thread (via start() )
        void run();

    private:
        QMap<quint16,AbstractProtocol* > _protocolPortMap;
};

} // namespace pelican
#endif // PELICANSERVER_H 
