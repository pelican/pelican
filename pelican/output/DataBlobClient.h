#ifndef DATABLOBCLIENT_H
#define DATABLOBCLIENT_H


#include <QObject>
#include <QSet>
#include <QString>
class QTcpSocket;

/**
 * @file DataBlobClient.h
 */

namespace pelican {
    class AbstractClientProtocol;
    class ConfigNode;
    class ServerRequest;
    class DataBlob;

/**
 * @class DataBlobClient
 *  
 * @brief
 *   A client for connecting to PelicanTCPBlobServer
 * @details
 * @example configuration
 * <server host="hostname" port="1234">
 */

class DataBlobClient : public QObject
{
    Q_OBJECT

    public:
        DataBlobClient( const ConfigNode&, QObject* parent = 0 );
        ~DataBlobClient();

        /// returns the streams served by the blob server
        QSet<QString> streams();

        /// listen for the named streams
        void subscribeToStreams( const QSet<QString>& streams );

        /// set the host to listen to
        void setIP_Address(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

    protected:
        void _sendRequest( const ServerRequest* req ) const;
        void _response();

    signals:
        void newData(DataBlob*);
        void newStreamsAvailable();

    private:
        AbstractClientProtocol* _protocol;
        QTcpSocket* _tcpSocket;
        QString     _server;
        quint16     _port;
        QSet<QString> _streams;
        mutable bool _streamInfo; // marker to test if stream response has been received
};

} // namespace pelican
#endif // DATABLOBCLIENT_H 
