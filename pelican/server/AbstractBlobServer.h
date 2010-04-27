#ifndef ABSTRACTBLOBSERVER_H
#define ABSTRACTBLOBSERVER_H
#include <QObject>

class QBuffer;

/**
 * @file AbstractBlobServer.h
 */

namespace pelican {
    class DataBlob;

/**
 * @class AbstractBlobServer
 *  
 * @brief
 *    Base class for server mechanisms to Serve DataBlobs
 * @details
 * 
 */

class AbstractBlobServer : public QObject
{
    Q_OBJECT

    public:
        AbstractBlobServer( QObject* parent = 0 );
        virtual ~AbstractBlobServer();
        virtual void queue(const QString& streamName, DataBlob*);

        virtual void send(const QString& streamName,const QBuffer&) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTBLOBSERVER_H 
