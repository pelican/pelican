#ifndef TCPCONNECTIONMANAGER_H
#define TCPCONNECTIONMANAGER_H

#include <QtNetwork>
#include <QThread>
#include <QMutex>
#include <QList>
#include <QMap>

#include "pelican/utility/ConfigNode.h"

/**
 * @file TCPConnectionManager.h
 */

namespace pelican {

/**
 * @class TCPConnectionManager
 *
 * @brief
 *   TCP Connection Management thread
 * @details
 *
 */

class TCPConnectionManager : public QThread
{
    Q_OBJECT

    public:
        TCPConnectionManager(const   ConfigNode& configNode, QObject *parent = 0);
        ~TCPConnectionManager();

    public:
        virtual void run();

    public:
        QMap<QString, QList<QTcpSocket*> >* getClientsReference() { return &_clients; }

    public slots:
        void send(const QString& streamName, const QByteArray& incoming);

    private:
        quint16                            _port;
        QMap<QString, QList<QTcpSocket*> > _clients;
        QTcpServer*                        _tcpServer;
        QMutex                             _mutex;

    private slots:
        void connectionError(QAbstractSocket::SocketError socketError);
        void acceptClientConnection();
        void connectionLost();
};

} // namespace pelican
#endif // TCPCONNECTIONMANAGER_H
