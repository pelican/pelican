#ifndef TCPCONNECTIONMANAGER_H
#define TCPCONNECTIONMANAGER_H

/**
 * @file TCPConnectionManager.h
 */

#include <QtNetwork/QTcpServer>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QMap>

#include "pelican/utility/ConfigNode.h"

namespace pelican {

class AbstractProtocol;
class DataBlob;

/**
 * @ingroup c_output
 *
 * @class TCPConnectionManager
 *
 * @brief
 *   TCP Connection Management thread
 * @details
 *
 */

class TCPConnectionManager : public QObject
{
    Q_OBJECT

    public:
        TCPConnectionManager(quint16 port=0, QObject *parent = 0);
        ~TCPConnectionManager();
        qint16 serverPort() const;
        int clientsForStream(const QString&) const;
        // stop listening for new connections, but preserve state
        void stop();
        // start listening for new connections, after a stop()
        void listen();

    protected:
        virtual void run();
        void _killClient(QTcpSocket*);
        const QSet<QString>& types() const;
        void _sendNewDataTypes();
        bool _processIncomming(QTcpSocket*);

    public slots:
        void send(const QString& streamName, const DataBlob* incoming);

    private:
        typedef QList<QTcpSocket*>         clients_t;
        quint16                            _port;
        QMap<QString, clients_t >          _clients;
        QTcpServer*                        _tcpServer;
        QMutex                             _mutex; // controls access to _clients
        QMutex                             _sendMutex; // controls access to send method
        AbstractProtocol*                  _protocol;
        QSet<QString>                      _seenTypes;          // record what types have been seen (via send() )
        const QString                      _dataSupportStream;  // the name of the subscrition stream for data support requests

    private slots:
        void connectionError(QAbstractSocket::SocketError socketError);
        void acceptClientConnection();
        void _incomingFromClient();

    signals:
        void sent(const DataBlob*);

};

} // namespace pelican
#endif // TCPCONNECTIONMANAGER_H
