#ifndef SESSION_H
#define SESSION_H

#include <QThread>
#include <QList>
class QTcpSocket;

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
        Session(int socketDescriptor, AbstractProtocol* proto, DataManager* data, QObject* parent=0 );
        ~Session();
        void run();
        void processRequest(const ServerRequest&, QDataStream& );
        QList<LockedData> processStreamDataRequest(const StreamDataRequest& req );

    private:
        int _socketDescriptor;
        DataManager* _data;
        AbstractProtocol* _proto;
};

} // namespace pelican
#endif // SESSION_H 
