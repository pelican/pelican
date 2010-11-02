#ifndef THREADEDBLOBSERVER_H
#define THREADEDBLOBSERVER_H

/**
 * @file ThreadedBlobServer.h
 */

#include <QtCore/QThread>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <boost/shared_ptr.hpp>

namespace pelican {
class DataBlob;
class TCPConnectionManager;

/**
 * @ingroup c_output
 *
 * @class ThreadedBlobServer
 *
 * @brief
 *   Runs a TCPConnection manager in a seperate thread
 * @details
 *
 */

class ThreadedBlobServer : public QThread
{
    Q_OBJECT

    public:
        ThreadedBlobServer( quint16 port, QObject* parent=0 );
        ~ThreadedBlobServer();

        /// send in a seperate background
        void send(const QString& streamName, const DataBlob* incoming);

        /// send and block until sent
        void blockingSend(const QString& streamName, const DataBlob* incoming);

        /// return the port on which the server is listening
        qint16 serverPort() const;

        /// returns the number of clients listening for the specified stream
        int clientsForStream(const QString&) const;

        /// stop the server from accepting new connections
        void stop();

        /// restart the serveraafter a stop()
        void listen();

    protected:
        void run();

    signals:
        /// private signal to communicate internally
        void sending( const QString& , const DataBlob*);

    private slots:
        void sent(const DataBlob*);

    private:
        boost::shared_ptr<TCPConnectionManager> _manager;
        quint16 _port;
        QMap<const DataBlob*, QWaitCondition*> _waiting;
        QMutex _mutex;

};

} // namespace pelican
#endif // THREADEDBLOBSERVER_H
