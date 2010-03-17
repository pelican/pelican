#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QList>
#include <QString>
#include <QThread>
#include "server/DataManager.h"

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
 *    packets. Data to be served by the server is passed through by
 *    the serveData() methods. The server uses the full Pelican Server
 *    infrastructure except wil a specialist DataManager.
 */

class TestServer : public QThread
{
    Q_OBJECT

    public:
        TestServer( AbstractProtocol* proto = 0 , QObject* parent =0);
        ~TestServer();

        /// return the port number the server is listening on
        quint16 port() const;
        bool isListening() const;

        /// Set Data to be Served
        void serveStreamData(const QList<StreamData>& );
        void serveStreamData(const StreamData& );
        void serveServiceData(const QList<Data>& );
        void serveServiceData(const Data&);

   protected:
        virtual void run();

    private:
        bool _protoOwner; // flag to indicate ownership of the 
                          // protocol object
        AbstractProtocol* _proto;
        DataManager _dm;
        PelicanPortServer* _server;

};

} // namespace pelican
#endif // TESTSERVER_H 
