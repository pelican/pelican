/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "PelicanServerClient.h"
#include "core/AbstractServiceAdapter.h"
#include "core/AbstractStreamAdapter.h"
#include "data/DataSpec.h"
#include "data/DataBlob.h"
#include "comms/ServerResponse.h"
#include "comms/DataBlobResponse.h"
#include "comms/StreamData.h"
#include "comms/StreamDataRequest.h"
#include "comms/ServiceDataRequest.h"
#include "comms/StreamDataResponse.h"
#include "comms/ServiceDataResponse.h"
#include "comms/PelicanClientProtocol.h"
#include "comms/DataSupportRequest.h"
#include "comms/DataSupportResponse.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <QtCore/QHash>
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <QtCore/QDebug>

#include <vector>
#include <iostream>
using std::cout;
using std::endl;

namespace pelican {

/**
 * Creates a Pelican server client for communication with a PelicanServer.
 * Communications are made using the PelicanClientProtocol on the host address
 * and port specified in the configuration node.
 *
 * @param configNode
 * @param types
 * @param config
 */
PelicanServerClient::PelicanServerClient(const ConfigNode& configNode,
        const DataTypes& types, const Config* config
        )
    : AbstractAdaptingDataClient(configNode, types, config)
        , _protocol(0), _specRecieved(false)
{
    _protocol = new PelicanClientProtocol;

    setIP_Address(configNode.getOption("server", "host"));
    setPort(configNode.getOption("server", "port").toUInt());
}


/**
 * @details
 */
PelicanServerClient::~PelicanServerClient()
{
    delete _protocol;
}

/**
 * @details
 */
void PelicanServerClient::setPort(unsigned port)
{
    _port = port;
}

/**
 * @details
 */
void PelicanServerClient::setIP_Address(const QString& ipaddress)
{
    _server = ipaddress;
}

/**
 * @details
 */
AbstractDataClient::DataBlobHash PelicanServerClient::getData(DataBlobHash& dataHash)
{
    QSet<QString> reqs = _requireSet;
    if( ! reqs.subtract(QSet<QString>::fromList(dataHash.keys())).isEmpty() )
        throw(QString("PelicanServerClient::getData() data hash does not "
                "contain objects for all possible requests"));

    // Construct the request
    StreamDataRequest sr;
    foreach(const DataSpec& d, dataRequirements())
    {
        sr.addDataOption( d );
    }

    DataBlobHash validData;

    // If the request isn't empty, send a request to the server and handle the
    // response only returning after adapting valid data.
    // \todo why is this all done in a single _sendRequest call?!
    if(!sr.isEmpty())
        validData.unite(_sendRequest(sr, dataHash));
    else
        throw QString("PelicanServerClient::getData(): Request for non-stream data");

    return validData;
}


/**
 * @details
 *
 * @param request
 * @param dataHash
 * @return
 */
AbstractDataClient::DataBlobHash PelicanServerClient::_sendRequest(
        const ServerRequest& request, DataBlobHash& dataHash)
{
    // Connect to the server.
    DataBlobHash validData;
    QTcpSocket sock;
    validData = _response(sock, _sendRequest( sock, request ), dataHash);

    return validData;
}

boost::shared_ptr<ServerResponse> PelicanServerClient::_sendRequest( QTcpSocket& sock, const ServerRequest& request ) const {
    Q_ASSERT(_server != "");
    sock.connectToHost(_server, _port , QIODevice::ReadWrite);
    while(! sock.waitForConnected(-1))
    {
        QAbstractSocket::SocketError e = sock.error();
        if( e != QAbstractSocket::ConnectionRefusedError
         && e != QAbstractSocket::RemoteHostClosedError
         && e != QAbstractSocket::QAbstractSocket::NetworkError ) {
            throw(QString("PelicanServerClient: unable to connect to host ") + _server
                + QString(" port %1").arg( _port) + " : " + sock.errorString() );
        }
        sleep(4); // wait before trying again
        sock.connectToHost(_server, _port , QIODevice::ReadWrite);
    }

    // Write the request to the open TCP socket with the PelicanClientProtocol.
    sock.write(_protocol->serialise(request));
    sock.flush();
    while (sock.bytesToWrite() > 0)
        sock.waitForBytesWritten(-1);

    // Receive the response from the server and process it.
    sock.waitForReadyRead(-1); // Need to supply -1 so this doesn't time out.
    return _protocol->receive(sock);
}

/**
 * @details
 * Process the server response. Server responses take the form of a number of
 * recognised types:
 *
 *  - Server Error.
 *  - Blob: A data blob returned from the server.
 *  - StreamData: A set of stream data returned from the server.
 *  - ServiceData: A set of service data returned from the server.
 *
 * Depending on the return type the response is processed to populate a
 * hash of data blobs matching the data requested.
 *
 *
 *
 * @param device   QIODevice containing the data returned from the server.
 * @param r        The server response object.
 * @param dataHash Hash of data blob pointers which are availiable before the
 *                 request.
 *
 * @return Hash of data blobs that are valid and ready to be used by pipelines.
 */
AbstractDataClient::DataBlobHash
PelicanServerClient::_response(QIODevice& device, shared_ptr<ServerResponse> r,
        DataBlobHash& dataHash)
{
    // Data hash of valid data to return.
    DataBlobHash validData;

    // Switch on the type of response from the server.
    switch(r->type())
    {
        case ServerResponse::Error:
        {
            QString msg = "PelicanServerClient: Server Error: " + r->message();
            std::cerr << msg.toStdString() << std::endl;
            throw( msg );
            break;
        }

        case ServerResponse::StreamData:
        {
            StreamDataResponse* resp = static_cast<StreamDataResponse*>(r.get());

            // Retrieve the stream data set.
            StreamData* sd = resp->streamData();
            Q_ASSERT(sd != 0);

            // Determine the set of associated service data (if any).
            // This id done by looping over the stream data object's associated
            // data and appending it to the data hash.
            ServiceDataRequest req;
            foreach (const shared_ptr<DataChunk>& d, sd->associateData())
            {
                // If the service data is already available at the correct
                // version, no need to fetch it from the server but simply
                // mark the current version valid.
                if( dataHash[d->name()]->version() == d->id()) {
                    validData[d->name()] = dataHash[ d->name()];
                }
                // Data is not available so fetch it from the server.
                else {
                    req.request(d->name(), d->id());
                }
            }

            // Retrieve the stream data.
            if(req.isEmpty()) {
                // If there is no service data to fetch so we can adapt the
                // stream data immediately.
                validData.unite(_adaptStream(device, sd, dataHash));
            }
            else {
                // There is service data to retrieve therefore we need to
                // fetch the required service data before adapting the stream
                // data, as the adapter may require the contents of the
                // service data to interpret the incoming data stream

                // Read the stream data out of the QIOdevice into a temporary buffer.
                std::vector<char> tmp(sd->size());
                int timeout = 2000;
                int bytesRead = 0;
                unsigned long bytesReadTotal = 0;

                while(bytesReadTotal != sd->size())
                {
                    while (device.bytesAvailable() < 1) {
                        if (!device.waitForReadyRead(timeout)) {
                            log(QString("PelicanServerClient: Timed out"
                                    " from server.") + device.errorString());
                            return validData;
                        }
                    }
                    bytesRead = device.read(&tmp[0 + bytesReadTotal], sd->size());
                    if(bytesRead == -1) {
                        log(QString("PelicanServerClient: Problem reading "
                                "from server.") + device.errorString());
                        return validData;
                    }
                    bytesReadTotal += bytesRead;
                }
                Q_ASSERT(tmp.size() == sd->size());

                // Fetch the service data.
                validData.unite(_getServiceData(req, dataHash));

                // Now we can adapt the stream data.
                QByteArray tmp_array = QByteArray::fromRawData(&tmp[0], tmp.size());
                QBuffer buf(&tmp_array);
                buf.open(QIODevice::ReadOnly);
                validData.unite(_adaptStream(buf, sd, dataHash));
            }
            break;
        }

        case ServerResponse::ServiceData:
        {
            // Service data
            ServiceDataResponse* res = static_cast<ServiceDataResponse*>(r.get());
            foreach(const DataChunk* d, res->data())
            {
                /*
                    QString type = d->name();
                    AbstractServiceAdapter* adapter = serviceAdapter(type);
                    Q_ASSERT(adapter != 0 );
                    dataHash[type]->setVersion(d->id());
                    adapter->config(dataHash[type], d->size() );
                    adapter->deserialise(&device);
                    validData.insert(type, dataHash.value(type));
                 */
                validData.unite(adaptService(device, d, dataHash));
            }
            break;
        }

        // Response containing a serialised data blob.
        case ServerResponse::Blob:
        {
            DataBlobResponse* resp = static_cast<DataBlobResponse*>(r.get());

            validData[resp->dataName()] = dataHash[resp->dataName()];

            // Deserialise the data blob into the data hash.
            while (device.bytesAvailable() < (qint64)resp->dataSize()) {
                device.waitForReadyRead(-1);
            }

            validData[resp->dataName()]->deserialise(device, resp->byteOrder());

            Q_ASSERT(resp->blobClass() == validData[resp->dataName()]->type());
            break;
        }

        default:
            std::cerr << "PelicanServerClient: Unknown Response" << std::endl;
            break;
    }
    return validData;
}


AbstractDataClient::DataBlobHash PelicanServerClient::_getServiceData(
        const ServiceDataRequest& request, DataBlobHash& dataHash)
{
    return _sendRequest(request, dataHash);
}


AbstractDataClient::DataBlobHash PelicanServerClient::_adaptStream(
        QIODevice& device, const StreamData* sd, DataBlobHash& dataHash)
{
    return adaptStream(device, sd, dataHash);
}

const DataSpec& PelicanServerClient::dataSpec() const {
    if( ! _specRecieved ) {
        // send a request to the server for the types of data
        DataSupportRequest request;
        QTcpSocket sock;
        boost::shared_ptr<ServerResponse> r = _sendRequest( sock, request );
        Q_ASSERT( r->type() == ServerResponse::DataSupport);
        DataSupportResponse* res = static_cast<DataSupportResponse*>(r.get());
        _dataSpec.clear();
        _dataSpec.addServiceData( res->serviceData() );
        _dataSpec.addStreamData( res->streamData() );
        _dataSpec.addAdapterTypes( res->defaultAdapters() );
        _specRecieved = true;
    }
    return _dataSpec;
}

} // namespace pelican
