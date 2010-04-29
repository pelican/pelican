#ifndef ABSTRACTBLOBSERVER_H
#define ABSTRACTBLOBSERVER_H
#include <QThread>
#include <QObject>

class QBuffer;

#include "pelican/server/DataManager.h"

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

class AbstractBlobServer : public QThread
{
    Q_OBJECT

    public:
        /// comment
        AbstractBlobServer( QObject* parent = 0 );
        /// comment
        virtual ~AbstractBlobServer();
        /// comment
        virtual void queue(const QString& streamName, DataBlob*);
        /// comment
        virtual void send(const QString& streamName,const QByteArray&);

    protected:
        DataManager* _dataManager;

    private:
};

} // namespace pelican
#endif // ABSTRACTBLOBSERVER_H 
