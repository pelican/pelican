#include "pelican/core/DirectStreamDataClient.h"

#include "pelican/core/PipelineApplication.h"
#include "pelican/comms/StreamData.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"
#include "pelican/server/ChunkerManager.h"
#include "pelican/server/LockedData.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/utility/Config.h"

#include <QBuffer>
#include <QCoreApplication>

#include "pelican/utility/memCheck.h"

namespace pelican {

PELICAN_DECLARE_CLIENT(DirectStreamDataClient)

class ConfigNode;

/**
 * @details Constructs the DirectStreamDataClient.
 */
DirectStreamDataClient::DirectStreamDataClient(const ConfigNode& configNode)
    : AbstractDataClient(configNode)
{
    // Initialise members.
    _started = false;
    _chunkerManager = 0;
    _dataManager = 0;
}

/**
 * @details Destroys the DirectStreamDataClient.
 */
DirectStreamDataClient::~DirectStreamDataClient()
{
    // Delete managers.
    delete _chunkerManager;
    delete _dataManager;
}

///**
// * @details
// * Sets the chunker for the given data type (DEPRECATED).
// *
// * @param dataType     The type of data.
// * @param chunkerType  The type of the chunker to use for the data.
// * @param chunkerName  The name of the chunker to use (optional).
// */
//void DirectStreamDataClient::setChunker(const QString& dataType,
//        const QString& chunkerType, const QString& chunkerName)
//{
//    switch (type(dataType)) {
//    case (AbstractAdapter::Stream):
//        _chunkerManager->addStreamChunker(chunkerType, chunkerName);
//        break;
//    case (AbstractAdapter::Service):
//        _chunkerManager->addServiceChunker(chunkerType, chunkerName);
//        break;
//    default:
//        throw QString("Data type %1 not set in requirements.").arg(dataType);
//    }
//}

/**
 * @details
 * Adds the given service chunker.
 *
 * @param dataType     The type of data.
 * @param chunkerType  The type of the chunker to use for the data.
 * @param chunkerName  The name of the chunker to use (optional).
 */
void DirectStreamDataClient::addServiceChunker(const QString& chunkerType,
        const QString& chunkerName)
{
    _chunkerManager->addServiceChunker(chunkerType, chunkerName);
}

/**
 * @details
 * Adds the given stream chunker.
 *
 * @param dataType     The type of data.
 * @param chunkerType  The type of the chunker to use for the data.
 * @param chunkerName  The name of the chunker to use (optional).
 */
void DirectStreamDataClient::addStreamChunker(const QString& chunkerType,
        const QString& chunkerName)
{
    _chunkerManager->addStreamChunker(chunkerType, chunkerName);
}


void DirectStreamDataClient::start()
{
    if( ! _started ) {
        _started = true;
        _chunkerManager->init(*_dataManager);
    }
}


QHash<QString, DataBlob*> DirectStreamDataClient::getData(QHash<QString, DataBlob*>& dataHash)
{
    QHash<QString, DataBlob*> validData;
    QSet<QString> reqs = _requireSet;
    if( ! reqs.subtract(QSet<QString>::fromList(dataHash.keys())).isEmpty() )
        throw(QString("DirectStreamDataClient::getData() Data hash does not "
                "contain objects for all possible requests"));

    // Check that the client is ready.
    if (!_started)
        throw QString("DirectStreamDataClient::getData(): Call start() first.");

    // keep polling the data manager until we can match a suitable request
    QList<LockedData> dataList; // will contain the list of valid StreamDataObjects
    while( dataList.size() == 0 ) {
        // Must cycle the event loop for unlocked signals to be transmitted.
        QCoreApplication::processEvents();
        QList<DataRequirements>::const_iterator it = dataRequirements().begin();
        while( it != dataRequirements().end() && dataList.size() == 0 )
        {
            if( ! it->isCompatible( _dataManager->dataSpec() ) )
                throw QString("DirectStreamDataClient::getData(): "
                        "Data requested not supported by client");
            dataList = _dataManager->getDataRequirements(*it);
            ++it;
        }
    }

    // Transform the Data  into DataBlobs
    for( int i=0; i < dataList.size(); ++i ) {
        LockableStreamData* lockedData = static_cast<LockableStreamData*>(dataList[i].object());
        StreamData* sd = lockedData->streamData();
        Q_ASSERT( sd != 0 );
        // check for associate data
        foreach (const boost::shared_ptr<Data>& d, sd->associateData()) {
            if( dataHash[d->name()]->version() == d->id())
            {
                // data not changed
                validData[d->name()] = dataHash[ d->name()];
            }
            else {
                // send the associate data through the adapter
                QByteArray tmp_array = QByteArray::fromRawData((char*)d->operator*(), d->size());
                QBuffer device(&tmp_array);
                device.open(QIODevice::ReadOnly);
                validData.unite(adaptService( device, d.get(), dataHash ));
            }
        }
        // send the data for adaption
        QByteArray tmp_array = QByteArray::fromRawData((char*)sd->operator*(), sd->size());
        QBuffer device(&tmp_array);
        device.open(QIODevice::ReadOnly);
        validData.unite(adaptStream( device, sd, dataHash ));

        static_cast<LockableStreamData*>(dataList[i].object())->served() = true;
    }

    return validData;
}

/**
 * @details
 * Creates the chunker manager.
 *
 * @param[in] config Pointer to the application's configuration object.
 */
void DirectStreamDataClient::setManagers(const Config* config)
{
    // Store a reference to the configuration node.
    _config = config;

    // Create the chunker manager.
    _dataManager = new DataManager(config, QString("pipeline"));
    _chunkerManager = new ChunkerManager(config, QString("pipeline"));
}

} // namespace pelican
