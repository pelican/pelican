#ifndef PELICANSERVERCLIENT_H
#define PELICANSERVERCLIENT_H

/**
 * @file PelicanServerClient.h
 */

#include "AbstractAdaptingDataClient.h"
#include <boost/shared_ptr.hpp>
#include "pelican/data/DataSpec.h"

using boost::shared_ptr;
class QTcpSocket;

namespace pelican {

class AbstractClientProtocol;
class ConfigNode;
class ServerRequest;
class ServerResponse;
class StreamData;
class ServiceDataRequest;

/**
 * @ingroup c_core
 *
 * @class PelicanServerClient
 *
 * @brief
 * Implements the data client interface for attaching to a Pelican Server.
 *
 * @details
 */

class PelicanServerClient : public AbstractAdaptingDataClient
{
    public:
        /// Construct and initialise the Pelican server client.
        PelicanServerClient(const ConfigNode& configNode,
                const DataTypes& types, const Config* config
                );

        virtual ~PelicanServerClient();

    public:
        /// Retrieve and adapt data required by the pipelines.
        virtual DataBlobHash getData(DataBlobHash& dataHash);
        virtual const DataSpec& dataSpec() const;

        /// Sets the port of the Pelican server being connected to.
        void setPort (unsigned port);

        /// Sets the IP address used of the Pelican server being connected to.
        void setIP_Address (const QString& ipaddress);

    protected: /// \todo why protected not private?
        /// Send a request to a PelicanServer for required data.
        DataBlobHash _sendRequest(const ServerRequest& request,
                DataBlobHash& dataHash);

        /// mechanics of sending a request and waiting for a response
        boost::shared_ptr<ServerResponse> _sendRequest( QTcpSocket& sock, const ServerRequest& request ) const;

        /// Process the response from the server.
        DataBlobHash _response(QIODevice&, shared_ptr<ServerResponse> r,
                DataBlobHash&);

        /// Sends a request for service data.
        DataBlobHash _getServiceData(const ServiceDataRequest& requirements,
                DataBlobHash& dataHash);

        /// Calls adaptStream on the data client base class.
        DataBlobHash _adaptStream(QIODevice& device, const StreamData*,
                DataBlobHash& dataHash);

    private:
        AbstractClientProtocol* _protocol;
        QString _server;
        unsigned _port;
        mutable DataSpec _dataSpec;

    private:
        /// Unit testing class.
        friend class PelicanServerClientTest;
};

PELICAN_DECLARE_CLIENT(PelicanServerClient)

} // namespace pelican
#endif // PELICANSERVERCLIENT_H
