#ifndef SESSION_H
#define SESSION_H

#include <QThread>
class QTcpSocket;

/**
 * @file Session.h
 */

namespace pelican {

class ServerRequest;
class AbstractProtocol;

/**
 * @class Session
 *  
 * @brief
 *    Class to process a single server request
 * @details
 * 
 */

class Session : public QThread
{
    Q_OBJECT

    public:
        Session(int socketDescriptor, AbstractProtocol* proto, QObject* parent=0 );
        ~Session();
        void run();
        void processRequest(const ServerRequest&, QDataStream& );

    private:
        int _socketDescriptor;
        AbstractProtocol* _proto;
};

} // namespace pelican
#endif // SESSION_H 
