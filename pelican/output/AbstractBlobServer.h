#ifndef ABSTRACTBLOBSERVER_H
#define ABSTRACTBLOBSERVER_H
#include <QThread>
#include <QObject>

class QBuffer;

#include "pelican/server/DataManager.h"
#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/ConfigNode.h"

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

class AbstractBlobServer: public AbstractModule
{

    public:
        /// comment
        AbstractBlobServer(const ConfigNode& configNode): AbstractModule(configNode) { }
        /// comment
        virtual ~AbstractBlobServer() { }
        /// comment
        virtual void send(const QString& streamName, const DataBlob* dataBlob) = 0;

    protected:
        DataManager* _dataManager;

    private:
};

} // namespace pelican
#endif // ABSTRACTBLOBSERVER_H 
