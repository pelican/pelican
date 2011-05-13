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
 * @ingroup c_server
 *
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

        void setVerbosity(int level) { _verboseLevel=level; };

    protected:
        /// Reimplemented from QTcpServer.
        void incomingConnection(int socketDescriptor);

    private:
        AbstractProtocol* _proto;
        DataManager* _data;
        int _verboseLevel;
};

} // namespace pelican
#endif // PELICANPORTSERVER_H
