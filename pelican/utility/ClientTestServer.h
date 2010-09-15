#ifndef CLIENTTESTSERVER_H
#define CLIENTTESTSERVER_H


#include <QThread>
#include <QString>
#include <QByteArray>
#include <QtNetwork/QTcpServer>

/**
 * @file ClientTestServer.h
 */

namespace pelican {

/**
 * @class ClientTestServer
 * 
 * @brief
 *    A server for unit testing client appliants.
 *    The test server will connect to an available port and record 
 *    any data sent to it
 * @details
 * 
 */
class ClientTestServer : public QThread
{
    Q_OBJECT

    public:
        /// ClientTestServer constructor.
        ClientTestServer( QObject* parent = 0 );

        /// ClientTestServer destructor.
        ~ClientTestServer();

        /// returns the hostname for the server
        const QString& hostname() const;

        /// return the port number the server is listening on
        //  A free port is assigned automatically.
        quint16 port() const;

        /// returns true if the server is listening for incomming
        //  requests
        bool isListening() const;

        /// returns a dump of all data received since the last call
        //  to this function
        QByteArray dataReceived();

    protected:
        void run();

    protected slots:
        void acceptClientConnection();
        void _incomingFromClient();
        void _connectionError(QAbstractSocket::SocketError);

    private:
        QByteArray _received;
        QString _hostname;
        QTcpServer* _server;
};

} // namespace pelican

#endif // CLIENTTESTSERVER_H 
