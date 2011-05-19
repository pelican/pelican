#ifndef SESSION_H
#define SESSION_H

#include <QtCore/QThread>
#include <QtCore/QList>
#include <QtNetwork/QTcpSocket>
#include <string>

/**
 * @file Session.h
 */

namespace pelican {

class ServerRequest;
class ServiceDataRequest;
class StreamDataRequest;
class LockedData;
class AbstractProtocol;
class DataManager;

/**
 * @ingroup c_server
 *
 * @class Session
 *
 * @brief
 * Class to process a single server request.
 *
 * @details
 *
 */
class Session : public QThread
{
    Q_OBJECT

    public:
        /// Constructs a server session.
        Session(int socketDescriptor, AbstractProtocol* proto, 
                DataManager* data,
                QObject* parent = 0);

        /// Destroys the server session.
        ~Session();

    public:
        /// Runs the session thread processing a request.
        /// (implements run method of QThread).
        void run();

        /// Process a request to the server sending the appropriate response.
        void processRequest(const ServerRequest&, QIODevice&, unsigned timeout = 0 );

        // set the verbosity level ( 0 = off )
        void setVerbosity(int level);

    protected:
        /// Returns the first valid stream data with associated service data.
        QList<LockedData> processStreamDataRequest(const StreamDataRequest& req,
                unsigned timeout = 0);

        QList<LockedData> processServiceDataRequest(const ServiceDataRequest& req);
        void verbose( const QString& msg, int verboseLevel = 1 );

    signals:
        void error(QTcpSocket::SocketError socketError);

    private:
        int _socketDescriptor;
        DataManager* _dataManager;
        AbstractProtocol* _protocol;
        int _verboseLevel;
        std::string _clientInfo;
        friend class SessionTest; // unit test
};

} // namespace pelican

#endif // SESSION_H
