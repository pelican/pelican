#ifndef PELICANSERVER_H
#define PELICANSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QMap>
#include <QSet>
#include <QPair>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

/**
 * @file PelicanServer.h
 */

namespace pelican {

class AbstractChunker;
class AbstractProtocol;
class ChunkerManager;
class Config;
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
        PelicanServer(const Config* config, QObject* parent=0);
        ~PelicanServer();

        /// Assosiate an incoming port with a particular protocol.
        /// Ownership of AbstractProtocol is transferred to this class.
        void addProtocol(AbstractProtocol*, quint16 port);

        /// Adds a stream chunker.
        void addStreamChunker(QString type, QString name = QString());

        /// Adds a service chunker.
        void addServiceChunker(QString type, QString name = QString());

        /// Returns true if the server is ready.
        bool isReady() {QMutexLocker locker(&_mutex); return _ready;}

    protected:
        // code to be executed in the thread ( via start() )
        void run();

    private:
        QMap<quint16,AbstractProtocol*> _protocolPortMap;
        QMutex _mutex;
        ChunkerManager* _chunkerManager;
        bool _ready;
        const Config* _config;
};

} // namespace pelican

#endif // PELICANSERVER_H 
