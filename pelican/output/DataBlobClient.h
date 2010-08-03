#ifndef DATABLOBCLIENT_H
#define DATABLOBCLIENT_H


#include <QObject>
#include <QSet>
#include <QString>
#include <QHash>
class QTcpSocket;

/**
 * @file DataBlobClient.h
 */

namespace pelican {
    class AbstractClientProtocol;
    class ConfigNode;
    class DataBlobFactory;
    class ServerRequest;
    class DataBlob;
    class Stream;

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
        void subscribe( const QSet<QString>& streams );
        void subscribe( const QString& stream );

        /// set the host to listen to
        void setHost(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

        /// return the port of the host connected to
        quint16 port() const;

    protected slots:
        void _response();

    protected:
        void _sendRequest( const ServerRequest* req );
        bool _connect();

        // return a data blob ready to be deserialised
        DataBlob* _blob(const QString& type, const QString& stream);

    signals:
        void newData(const Stream& stream);
        void newStreamsAvailable();

    private:
        QHash<QString, Stream*> _streamMap;
        AbstractClientProtocol* _protocol;
        QTcpSocket* _tcpSocket;
        DataBlobFactory* _blobFactory;
        QString     _server;
        quint16     _port;
        QSet<QString> _streams;
        mutable bool _streamInfo; // marker to test if stream response has been received
};

} // namespace pelican
#endif // DATABLOBCLIENT_H 
