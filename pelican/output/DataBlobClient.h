#ifndef DATABLOBCLIENT_H
#define DATABLOBCLIENT_H


#include "pelican/output/AbstractDataBlobClient.h"
#include "pelican/data/DataRequirements.h"
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

class DataBlobClient : public AbstractDataBlobClient
{

    Q_OBJECT

    public:
        DataBlobClient( const ConfigNode&, QObject* parent = 0 );
        ~DataBlobClient();

        // base class interface requirements
        virtual QSet<QString> streams();

        /// listen for the named streams
        using AbstractDataBlobClient::subscribe;
        virtual void subscribe( const QSet<QString>& streams );

        /// set the host to listen to
        void setHost(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

        /// return the port of the host connected to
        quint16 port() const;

    protected slots:
        void _response();

        /// attempt to reconnect to the server and restore subscriptions
        //in case of a sudden disconnect
        void _reconnect();

    protected:
        bool  _sendRequest( const ServerRequest* req );
        bool _connect();

        /// return a data blob ready to be deserialised
        DataBlob* _blob(const QString& type, const QString& stream);

    private:
        bool _requestStreamInfo();

    protected:
        QSet<QString> _streams;

    private:
        QHash<QString, Stream*> _streamMap;
        AbstractClientProtocol* _protocol;
        QTcpSocket* _tcpSocket;
        DataBlobFactory* _blobFactory;
        QString       _server;
        quint16       _port;
        QSet<QString> _subscriptions;
        mutable bool  _streamInfo; // marker to test if stream response has been received
        mutable bool  _streamInfoSubscription;
        bool _destructor;
        DataRequirements _currentSubscription;

};

} // namespace pelican
#endif // DATABLOBCLIENT_H 
