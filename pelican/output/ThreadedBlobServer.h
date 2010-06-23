#ifndef THREADEDBLOBSERVER_H
#define THREADEDBLOBSERVER_H


#include <QThread>
#include <QString>
#include <boost/shared_ptr.hpp>

/**
 * @file ThreadedBlobServer.h
 */

namespace pelican {
    class DataBlob;
    class TCPConnectionManager;

/**
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
        void send(const QString& streamName, const DataBlob* incoming);
        qint16 serverPort() const;

    protected:
        void run();

    signals:
        /// private signal to communicate internally
        void sending( const QString& , const DataBlob*);

    private:
        boost::shared_ptr<TCPConnectionManager> _manager;
        quint16 _port;

};

} // namespace pelican
#endif // THREADEDBLOBSERVER_H 
