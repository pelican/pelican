#ifndef TESTSERVER_H
#define TESTSERVER_H

/**
 * @file TestServer.h
 */

#include "pelican/server/DataManager.h"

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QThread>

namespace pelican {

class PelicanPortServer;
class AbstractProtocol;

namespace test {

/**
 * @ingroup t_server
 *
 * @class TestServer
 *
 * @brief
 *    A class that sets up and runs a PelicanPortServer on the local host
 *    for testing purposes
 *
 * @details
 *    The TestServer will start listening on a random port for incomming
 *    packets as soon as it is contructed. Data to be served by the server
 *    is passed through by the serveData() methods. The server uses the
 *    full Pelican Server infrastructure.
 *
 *  \par Example:
 *  \code
 *    // Server setup.
 *    TestServer server;
 *    server.setServiceData( someServiceData );
 *
 *    // A mythical client interface.
 *    MyClient client;
 *    client.setPort( server.port() );
 *    client.fetch(someStreamDataIdentifier)
 *  \endcode
 */

class TestServer : public QThread
{
    Q_OBJECT

    public:
        TestServer( AbstractProtocol* proto = 0 , QObject* parent =0);
        ~TestServer();

        /// Return the port number the server is listening on.
        /// A free port is assigned automatically.
        quint16 port() const;

        /// returns true if the server is listening for incoming requests
        bool isListening() const;

        /// Set Data to be Served.
        //  data will be served in the order provided
        //  and will be removed once the data has been served
        void serveStreamData(const QList<StreamData>& );
        void serveStreamData(const StreamData& );
        void serveServiceData(const QList<DataChunk>& );
        void serveServiceData(const DataChunk&);

   protected:
        virtual void run();

    private:
        bool _protoOwner; // flag to indicate ownership of the
                          // protocol object
        DataManager* _dataManager;
        AbstractProtocol* _proto;
        PelicanPortServer* _portServer;
};

} // namespace test
} // namespace pelican
#endif // TESTSERVER_H
