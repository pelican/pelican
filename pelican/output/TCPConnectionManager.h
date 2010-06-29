#ifndef TCPCONNECTIONMANAGER_H
#define TCPCONNECTIONMANAGER_H

#include <QtNetwork/QTcpServer>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QList>
#include <QtCore/QMap>

#include "pelican/utility/ConfigNode.h"

/**
 * @file TCPConnectionManager.h
 */

namespace pelican {

    class AbstractProtocol;
    class DataBlob;

/**
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
        int clientsForType(const QString&) const;

    protected:
        virtual void run();
        void _killClient(QTcpSocket*);

    public slots:
        void send(const QString& streamName, const DataBlob* incoming);

    private:
        quint16                            _port;
        typedef QList<QTcpSocket*>         clients_t;
        QMap<QString, clients_t > _clients;
        QTcpServer*                        _tcpServer;
        QMutex                             _mutex; // controls access to _clients
        QMutex                             _sendMutex; // controls access to send method
        AbstractProtocol*                  _protocol;

    private slots:
        void connectionError(QAbstractSocket::SocketError socketError);
        void acceptClientConnection();

    signals:
        void sent(const DataBlob*);

};

} // namespace pelican
#endif // TCPCONNECTIONMANAGER_H
