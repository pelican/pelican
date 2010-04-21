#ifndef PELICANSERVERCLIENT_H
#define PELICANSERVERCLIENT_H

#include <boost/shared_ptr.hpp>
#include "AbstractDataClient.h"

/**
 * @file PelicanServerClient.h
 */

namespace pelican {

class AbstractClientProtocol;
class ConfigNode;
class ServerRequest;
class ServerResponse;
class StreamData;
class ServiceDataRequest;

/**
 * @class PelicanServerClient
 *
 * @brief
 * Implements the data client interface for attaching to a Pelican Server
 *
 * @details
 *
 */

class PelicanServerClient : public AbstractDataClient
{
    public:
        PelicanServerClient (const ConfigNode& config);
        ~PelicanServerClient();
        virtual QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>& dataHash);
        void setPort (unsigned int port);
        void setIP_Address (const QString& ipaddress);

    protected:
        QHash<QString, DataBlob*> _getServiceData(const ServiceDataRequest& requirements, QHash<QString, DataBlob*>& dataHash);
        QHash<QString, DataBlob*> _sendRequest(const ServerRequest& request, QHash<QString, DataBlob*>& dataHash);
        QHash<QString, DataBlob*> _response(QIODevice&, boost::shared_ptr<ServerResponse> r, QHash<QString, DataBlob*>&);
        QHash<QString, DataBlob*> _adaptStream(QIODevice& device, const StreamData*, QHash<QString, DataBlob*>& dataHash);
        void _connect();

    private:
        AbstractClientProtocol* _protocol;
        QString _server;
        unsigned int _port;

        friend class PelicanServerClientTest;
};

} // namespace pelican

#endif // PELICANSERVERCLIENT_H 
