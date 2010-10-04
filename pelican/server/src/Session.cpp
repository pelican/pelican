#include "pelican/server/Session.h"

#include "pelican/comms/AbstractProtocol.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/LockableServiceData.h"
#include "pelican/comms/StreamData.h"
#include "pelican/comms/ServerRequest.h"
#include "pelican/comms/StreamDataRequest.h"
#include "pelican/comms/ServiceDataRequest.h"

#include <QtNetwork/QTcpSocket>
#include <QtCore/QString>
#include <QtCore/QHash>
#include <QtCore/QTime>

#include <iostream>

#include "pelican/utility/memCheck.h"

using std::cout;
using std::endl;

namespace pelican {

// class Session
Session::Session(int socketDescriptor, AbstractProtocol* proto, DataManager* data, QObject* parent)
    : QThread(parent), _dataManager(data)
{
    _protocol = proto;
    _socketDescriptor = socketDescriptor;
}

Session::~Session()
{
    wait();
}


/**
 * @details
 *
 */
void Session::run()
{
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(_socketDescriptor)) {
        emit error(socket.error());
        return;
    }

    boost::shared_ptr<ServerRequest> req = _protocol->request(socket);

    processRequest(*req, socket);
    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState)
        socket.waitForDisconnected();
}


/**
 * @details
 * This routine processes a general ServerRequest, calling the appropriate
 * working and then passes this on to the protocol to be returned to the client
 */
void Session::processRequest(const ServerRequest& req, QIODevice& out, const unsigned timeout)
{
    try {
        switch(req.type()) {
            case ServerRequest::Acknowledge:
                _protocol->send(out,"ACK");
                cout << "Session::processRequest(): Sent acknowledgement" << endl;
                break;
            case ServerRequest::StreamData:
                {
                    QList<LockedData> dataList = processStreamDataRequest(static_cast<const StreamDataRequest&>(req), timeout );
//                    cout << "Session::processRequest():  List size: " << dataList.size() << endl;
                    if( dataList.size() > 0 ) {
                        AbstractProtocol::StreamDataList data;
                        for( int i=0; i < dataList.size(); ++i )
                        {
                            LockableStreamData* lockedData = static_cast<LockableStreamData*>(dataList[i].object());
                            data.append(static_cast<StreamData*>(lockedData->streamData()));
                        }
                        _protocol->send( out, data );

                        // Mark as data as being served so it can be deactivated.
                        foreach (LockedData d, dataList) {
                            static_cast<LockableStreamData*>(d.object())->served() = true;
                        }
//                         cout << "Session::processRequest(): Sent stream data" << endl;
                    }
//                     cout << "Session::processRequest(): Finished stream data request" << endl;
                }
                break;
            case ServerRequest::ServiceData:
                {
                    QList<LockedData> d = processServiceDataRequest(static_cast<const ServiceDataRequest&>(req) );
                    if( d.size() > 0 ) {
                        AbstractProtocol::ServiceDataList data;
                        for( int i=0; i < d.size(); ++i ) {
                            LockableServiceData* lockedData = static_cast<LockableServiceData*>(d[i].object());
                            data.append(lockedData->data().get());
                        }
                        _protocol->send( out, data );
//                         cout << "Session::processRequest(): Sent service data" << endl;
                    }
                }
                break;
            default:
                _protocol->sendError( out, req.message());
        }
    }
    catch( const QString& e )
    {
        _protocol->sendError( out, e );
//         cout << "Sent error: " << e.toStdString() << endl;
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
        return dataList;
    }

    // Start a timer to handle the timeout.
    QTime time;
    time.start();

    // Iterate until the data requirements can be satisfied.
    while (dataList.size() == 0) { // keep spinning until we either get data or time out
        if ((unsigned int)time.elapsed() > timeout && timeout > 0) {
            throw QString("Session::processStreamDataRequest():"
            " Request timed out after %1 ms.").arg(time.elapsed());
        }
        DataRequirementsIterator it = req.begin();
        while(it != req.end() && dataList.size() == 0) {
            dataList = _dataManager->getDataRequirements( *it );
            ++it;
        }
        usleep(1);
    }
    return dataList;
}

/**
 * @details
 * Returns all the data sets mentioned in the list (or throws if any one is missing).
 * The data will be returned as a locked container to ensure access
 * by other threads will be blocked.
 */
QList<LockedData> Session::processServiceDataRequest(const ServiceDataRequest& req )
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
