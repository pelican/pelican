#ifndef PELICANTCPBLOBSERVER_H
#define PELICANTCPBLOBSERVER_H

#include "pelican/output/TCPConnectionManager.h"
#include "pelican/output/AbstractOutputStream.h"

/**
 * @file PelicanTCPBlobServer.h
 */

namespace pelican {

class ConfigNode;
class DataBlob;
class ThreadedBlobServer;

/**
 * @ingroup c_output
 *
 * @class PelicanTCPBlobServer
 *
 * @brief
 *   Server that sends the same data from the queue to all connected peers
 * @details
 *   Specify the port to listen to with a connection tag
 *   e.g.
 *   @code
 *   <connection port="1234">
 *   @endcode
 *
 *   The server has two modes : threaded (default) or non-threaded
 *
 *   In Threaded node, the clients will be served by a seperate thread. This will give connecting
 *   clients a reasonable response time whatever other components of the system are doing.
 *
 *   Non-threaded mode allows you to eliminate the performance cost of running a seperate thread
 *   but the server can only respond to clients requests during the calling threads event loop.
 *   This is acceptable when this called very frequently e.g. in a very fast running pipeline.
 *   WARNING - this mode is not yet tested TODO
 *   set the threaded="false" attribute in the configuration file to select this mode
 */

class PelicanTCPBlobServer : public AbstractOutputStream
{
    public:
        PelicanTCPBlobServer( const ConfigNode& config );
        ~PelicanTCPBlobServer();
        quint16 serverPort() const;
        // stop the server from accepting connections
        void stop();
        // start listening for incomming connections
        void listen();

        /// return the number of clients listening to a specified stream
        int clientsForStream(const QString& stream) const;

    protected:
        virtual void sendStream(const QString& streamName, const DataBlob* dataBlob);

    private:
        ThreadedBlobServer* _server;
        TCPConnectionManager*  _connectionManager;

    private:
        friend class PelicanTCPBlobServerTest;

};

PELICAN_DECLARE(AbstractOutputStream, PelicanTCPBlobServer )

} // namespace pelican
#endif // PELICANTCPBLOBSERVER_H
