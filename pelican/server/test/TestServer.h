#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "pelican/server/DataManager.h"

#include <QList>
#include <QString>
#include <QThread>

/**
 * @file TestServer.h
 */

namespace pelican {

class PelicanPortServer;
class AbstractProtocol;

/**
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
 *    Example:
 *    // Server setup
 *    TestServer server;
 *    server.setServiceData( someServiceData );
 *
 *    // a mythical client interface
 *    MyClient client;
 *    client.setPort( server.port() );
 *    client.fetch(someStreamDataIdentifier)
 */

class TestServer : public QThread
{
    Q_OBJECT

    public:
        TestServer( AbstractProtocol* proto = 0 , QObject* parent =0);
        ~TestServer();

        /// return the port number the server is listening on
        //  A free port is assigned automatically.
        quint16 port() const;

        /// returns true if the server is listening for incomming
        //  requests
        bool isListening() const;

        /// Set Data to be Served
        //  data will be served in the order provided
        //  and will be removed once the data has been served
        void serveStreamData(const QList<StreamData>& );
        void serveStreamData(const StreamData& );
        void serveServiceData(const QList<Data>& );
        void serveServiceData(const Data&);

   protected:
        virtual void run();

    private:
        bool _protoOwner; // flag to indicate ownership of the
                          // protocol object
        DataManager* _dataManager;
        AbstractProtocol* _proto;
        PelicanPortServer* _portServer;

};

} // namespace pelican
#endif // TESTSERVER_H
