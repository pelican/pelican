#ifndef DATABLOBCLIENT_H
#define DATABLOBCLIENT_H

/**
 * @file DataBlobClient.h
 */

#include <boost/shared_ptr.hpp>
#include "pelican/output/AbstractDataBlobClient.h"

#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QHash>


namespace pelican {
class AbstractClientProtocol;
class ConfigNode;
class DataBlobFactory;
class ServerRequest;
class DataBlob;
class Stream;

/**
 * @ingroup c_output
 *
 * @class DataBlobClient
 *
 * @brief
 * A client for connecting to PelicanTCPBlobServer
 *
 * @details
 *
 * \par Example Config:
 * \code
 * <connection host="hostname" port="1234" >
 * <subscribe stream="streamName" />
 * \endcode
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

    protected:
        virtual void onSubscribe(const QString&);
        virtual void onReconnect();
        virtual void dataSupport( DataSupportResponse* );
        virtual void dataReceived( DataBlobResponse* );

        /// return a data blob ready to be deserialised
        boost::shared_ptr<DataBlob> _blob(const QString& type, const QString& stream);

    private:
        QHash<QString, Stream*> _streamMap;
        DataBlobFactory* _blobFactory;
        mutable bool  _streamInfo; // marker to test if stream response has been received
        mutable bool  _streamInfoSubscription;

};

} // namespace pelican
#endif // DATABLOBCLIENT_H
