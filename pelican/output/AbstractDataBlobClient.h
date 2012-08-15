#ifndef ABSTRACTDATABLOBCLIENT_H
#define ABSTRACTDATABLOBCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSet>
#include "pelican/data/DataRequirements.h"
class QTcpSocket;

/**
 * @file AbstractDataBlobClient.h
 */

namespace pelican {
    class DataBlob;
    class Stream;
    class ServerRequest;
    class ServerResponse;
    class DataSupportResponse;
    class DataBlobResponse;
    class AbstractClientProtocol;

/**
 * @ingroup c_output
 *
 * @class AbstractDataBlobClient
 *
 * @brief
 *   Base class for all clients of the
 *   Pelican DataBlob server
 * @details
 *   Also provides an api for ommunications with 
 *   the server
 */
class AbstractDataBlobClient : public QObject
{
    Q_OBJECT

    public:
        /// AbstractDataBlobClient constructor.
        AbstractDataBlobClient(QObject* parent=0);

        /// AbstractDataBlobClient destructor.
        virtual ~AbstractDataBlobClient();

        /// set the protocol to use
        //  (must be set before trying to connect)
        //  takes ownership of the protocol object
        void setProtocol( AbstractClientProtocol* p );

        /// set the host to listen to
        void setHost(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

        /// return the port of the host connected to
        quint16 port() const;

        /// returns the streams served by the blob server
        virtual QSet<QString> streams() { return _streams; };

        /// listen for the named streams
        virtual void subscribe( const QSet<QString>& streams );
        void subscribe( const QString& stream );
        QTcpSocket* socket() { return _tcpSocket; }

    protected:
        /// report verbose messages
        void verbose(const QString&, int level = 1);
        /// send a request to the server
        //  using the provided protocol
        bool sendRequest( const ServerRequest* req );

        // send server a request for StreamInfo
        bool requestStreamInfo();

        /// reimplement in your class
        //  for dealing with the various responses to 
        //  the server
        virtual void serverError( ServerResponse* );
        virtual void dataSupport( DataSupportResponse* );
        virtual void dataReceived( DataBlobResponse* ) {};
        virtual void unknownResponse( ServerResponse* );

        virtual void onReconnect() {};
        virtual void onSubscribe( const QString& ) {};

    signals:
        void newData(const Stream& stream);
        void newStreamsAvailable();

    private: // methods
        bool _connect();

    private slots:
        void _response();
        void _reconnect();

    protected:
        QTcpSocket*  _tcpSocket;
        int _verbose;
        QString       _server;
        quint16       _port;


    private:
        AbstractClientProtocol* _protocol;
        bool _destructor;
        DataRequirements _currentSubscription;
        QSet<QString> _subscriptions;
        QSet<QString> _streams;

};

} // namespace pelican

#endif // ABSTRACTDATABLOBCLIENT_H
