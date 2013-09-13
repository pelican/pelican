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

#include "server/Session.h"

#include "comms/AbstractProtocol.h"
#include "server/DataManager.h"
#include "server/LockedData.h"
#include "server/LockableStreamData.h"
#include "server/LockableServiceData.h"
#include "comms/StreamData.h"
#include "comms/DataSupportResponse.h"
#include "comms/ServerRequest.h"
#include "comms/StreamDataRequest.h"
#include "comms/ServiceDataRequest.h"

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QTime>

#include <iostream>

using std::cout;
using std::endl;

namespace pelican {

/**
 * @details
 *
 * @param socketDescriptor
 * @param proto
 * @param data
 * @param parent
 * @return
 */
Session::Session(int socketDescriptor, AbstractProtocol* proto,
        DataManager* data, QObject* parent)
: QThread(parent), _dataManager(data), _verboseLevel(0)
{
    _protocol = proto;
    _socketDescriptor = socketDescriptor;
}


Session::~Session()
{
    wait();
}

void Session::setVerbosity(int level)
{
     _verboseLevel = level;
}

void Session::verbose( const QString& msg, int verboseLevel )
{
    if( verboseLevel <= _verboseLevel )
        std::cout << _clientInfo
                  << msg.toStdString()
                  << std::endl;
}

/**
 * @details
 */
void Session::run()
{
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(_socketDescriptor)) {
        emit error(socket.error());
        return;
    }
    _clientInfo = "Session: " + socket.peerAddress().toString().toStdString() + ": ";

    boost::shared_ptr<ServerRequest> req = _protocol->request(socket);
    processRequest(*req, socket);
    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState)
        socket.waitForDisconnected();
}


/**
 * @details
 * Processes a general ServerRequest, calling the appropriate working and then
 * passes this on to the protocol to be returned to the client.
 */
void Session::processRequest(const ServerRequest& req, QIODevice& out,
        const unsigned timeout)
{
    try {
        switch(req.type())
        {
            case ServerRequest::Acknowledge:
            {
                _protocol->send(out,"ACK");
                verbose("Sent acknowledgement");
                break;
            }

            case ServerRequest::DataSupport:
            {
                verbose("DataSupport request received");
                DataSupportResponse r( _dataManager->dataSpec() );
                _protocol->send(out, r);
                break;
            }
            case ServerRequest::StreamData:
            {
                // List of data to be served.
                QList<LockedData> dataList = processStreamDataRequest(
                        static_cast<const StreamDataRequest&>(req), timeout);

                if (dataList.size() > 0)
                {
                    AbstractProtocol::StreamData_t data;
                    for (int i = 0; i < dataList.size(); ++i) {
                        LockableStreamData* lockedData =
                                static_cast<LockableStreamData*>(dataList[i].object());
                        data.append(static_cast<StreamData*>(lockedData->streamData()));
                    }
                    _protocol->send(out, data);

                    // Mark as data as being served so it can be de-activated.
                    foreach (LockedData d, dataList) {
                        static_cast<LockableStreamData*>(d.object())->served() = true;
                    }
                }
                break;
            }

            case ServerRequest::ServiceData:
            {
                verbose("ServiceData request received");
                QList<LockedData> d =
                        processServiceDataRequest(static_cast<const ServiceDataRequest&>(req));
                if (d.size() > 0) {
                    AbstractProtocol::ServiceData_t data;
                    for (int i=0; i < d.size(); ++i)
                    {
                        LockableServiceData* lockedData =
                                static_cast<LockableServiceData*>(d[i].object());
                        data.append(lockedData->dataChunk().get());
                    }
                    _protocol->send(out, data);
                }
                break;
            }
            default:
                verbose("protocol error: " + req.message());
                _protocol->sendError(out, req.message());
                break;
        }
    }
    catch (const QString& e)
    {
        verbose("caught error: " + e );
        _protocol->sendError(out, e);
    }
}


/**
 * @details
 * Iterates over the list of data options (requirements) provided in the request
 * until a valid data object is returned.
 *
 * Returns the first data set available (streams and associated service data)
 * that match the requirements.
 *
 * An empty request will return immediately with an empty list.
 *
 * WARNING: This function will block until valid data matching the request can
 * made.
 *
 * The data will be returned as a locked container to ensure access by other
 * threads will be blocked. This is achieved by a signal emitted when the
 * LockedData object goes out of scope.
 *
 * @param[in] req       StreamDataRequest object containing a data requirements
 *                      iterator.
 * @param[in] timeout   Timeout in milliseconds. (0 = do not timeout)
 *
 * @return A list of locked data containing streams data object and their
 *         associated service data for the first request that can be fully
 *         satisfied.
 */
QList<LockedData> Session::processStreamDataRequest(const StreamDataRequest& req,
        const unsigned timeout)
{
    // Return an empty list if there are no data requirements in the request.
    QList<LockedData> dataList;
    if (req.isEmpty()) {
        verbose("StreamData request is empty");
        return dataList;
    }

    // Start a timer to handle the timeout.
    QTime time;
    time.start();

    verbose("processing StreamData request");
    // Iterate until the data requirements can be satisfied.
    // keep spinning until we either get data or time out.
    while (dataList.size() == 0)
    {
        if ((unsigned int)time.elapsed() > timeout && timeout > 0) {
            throw QString("Session::processStreamDataRequest():"
            " Request timed out after %1 ms.").arg(time.elapsed());
        }
        DataSpecIterator it = req.begin();
        while(it != req.end() && dataList.size() == 0) {
            dataList = _dataManager->getDataRequirements(*it);
            ++it;
        }
        usleep(1);
    }
    verbose("finished processing StreamData request");
    return dataList;
}


/**
 * @details
 * Returns all the data sets mentioned in the list (or throws if any one is missing).
 * The data will be returned as a locked container to ensure access
 * by other threads will be blocked.
 */
QList<LockedData> Session::processServiceDataRequest(const ServiceDataRequest& req)
{
    QList<LockedData> data;
    foreach (QString type, req.types()) {
        LockedData d = _dataManager->getServiceData(type, req.version(type));
        if (!d.isValid())
            throw QString("Session: Data requested does not exist: "
                    "%1 %2").arg(type).arg(req.version(type));
        data.append(d);
    }
    return data;
}

} // namespace pelican
