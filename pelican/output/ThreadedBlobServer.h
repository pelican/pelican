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
        void send(const QString& streamName, const DataBlob& blob);
        qint16 serverPort() const;

    protected:
        void run();

    private slots:
        void _send();

    private:
        boost::shared_ptr<TCPConnectionManager> _manager;
        quint16 _port;
        QString _sendStream;
        const DataBlob* _sendBlob;

};

} // namespace pelican
#endif // THREADEDBLOBSERVER_H 
