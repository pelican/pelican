#ifndef PELICANPORTSERVER_H
#define PELICANPORTSERVER_H


#include <QTcpServer>

/**
 * @file PelicanPortServer.h
 */

namespace pelican {
class AbstractProtocol;

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
        PelicanPortServer(AbstractProtocol* proto, QObject* parent=0 );
        ~PelicanPortServer();

    protected:
        void incomingConnection(int socketDescriptor);

    private:
        AbstractProtocol* _proto;
};

} // namespace pelican
#endif // PELICANPORTSERVER_H 
