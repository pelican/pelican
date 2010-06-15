#ifndef PELICANPORTSERVER_H
#define PELICANPORTSERVER_H


#include <QtNetwork/QTcpServer>

/**
 * @file PelicanPortServer.h
 */

namespace pelican {
class AbstractProtocol;
class DataManager;

/**
 * @class PelicanPortServer
 *  
 * @brief
 *    Class that listens on a specific port
 * @details
 *    Internal class used by PelicanServer
 *    Each port is associated with a single protocol
 * 
 */
class Session;

class PelicanPortServer : public QTcpServer
{

    Q_OBJECT

    public:
        PelicanPortServer(AbstractProtocol* proto, DataManager* data, QObject* parent=0 );
        ~PelicanPortServer();

    protected:
        /// Reimplemented from QTcpServer.
        void incomingConnection(int socketDescriptor);

    private:
        AbstractProtocol* _proto;
        DataManager* _data;
};

} // namespace pelican
#endif // PELICANPORTSERVER_H 
