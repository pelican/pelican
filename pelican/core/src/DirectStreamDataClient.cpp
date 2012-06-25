#include "pelican/core/DirectStreamDataClient.h"

#include "pelican/core/PipelineApplication.h"
#include "pelican/comms/StreamData.h"
#include "pelican/data/DataRequirements.h"
#include "pelican/data/DataBlob.h"
#include "pelican/server/ChunkerManager.h"
#include "pelican/server/DataManager.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/LockedData.h"
#include "pelican/utility/Config.h"

#include <QtCore/QBuffer>
#include <QtCore/QCoreApplication>

namespace pelican {

class ConfigNode;

/**
 * @details Constructs the DirectStreamDataClient.
 */
DirectStreamDataClient::DirectStreamDataClient(const ConfigNode& configNode,
        const DataTypes& types, const Config* config
        )
        : AbstractAdaptingDataClient(configNode, types, config )
{
    // Initialise members.
    _started = false;

    // Create the managers.
    _dataManager = new DataManager(config, QString("pipeline"));
    _chunkerManager = new ChunkerManager(config, QString("pipeline"));
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

/**
 * @details
 * Sets the chunker for the given data type.
 *
 * @param dataType     The type of data.
 * @param chunkerType  The type of the chunker to use for the data.
 * @param chunkerName  The name of the chunker to use (optional).
 */
void DirectStreamDataClient::addChunker(const QString& dataType,
        const QString& chunkerType, const QString& chunkerName)
{
    switch (type(dataType))
    {
        case AbstractAdapter::Stream:
            _chunkerManager->addStreamChunker(chunkerType, chunkerName);
            break;

        case AbstractAdapter::Service:
            _chunkerManager->addServiceChunker(chunkerType, chunkerName);
            break;

        default:
            throw QString("DirectStreamDataClient::addChunker(): "
                    "Data type %1 not set in requirements.").arg(dataType);
    }
}

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

/**
 * @details
 * Gets the data from the data stream(s), chunks it into the buffer and
 * adapts it into data blobs ready for use by the pipeline.
 */
AbstractDataClient::DataBlobHash DirectStreamDataClient::getData(
        DataBlobHash& dataHash)
{
    // Initialise and perform sanity checks.
    if (!_started) {
        _init(dataHash);
        _started = true;
    }

    // Keep polling the data manager until we can match a suitable request.
    DataBlobHash validData;
    QList<LockedData> dataList; // Will contain the list of valid StreamDataObjects
    do {
        // Must cycle the event loop for unlocked signals to be processed.
        for (int i = 0; i < _nPipelines; ++i) {
            dataList = _dataManager->getDataRequirements(dataRequirements().at(i));
            if (dataList.size() != 0) break;
        }
        if( dataList.size() == 0 )
            QCoreApplication::processEvents( QEventLoop::WaitForMoreEvents, 10 );
            //QCoreApplication::processEvents( QEventLoop::AllEvents, 5 );
    }
    while (dataList.size() == 0 );

    // Transform the data into DataBlobs.
    for (int i = 0; i < dataList.size(); ++i) {
        LockableStreamData* lockedData = static_cast<LockableStreamData*>(dataList[i].object());
        StreamData* sd = lockedData->streamData();
        Q_ASSERT(sd != 0);
        // Check for associate data
        foreach (const boost::shared_ptr<DataChunk>& d, sd->associateData()) {
            if (dataHash[d->name()]->version() == d->id())
            {
                // Data not changed
                validData[d->name()] = dataHash[ d->name()];
            }
            else {
                // Send the associate data through the adapter
                QByteArray tmp = QByteArray::fromRawData((char*)d->ptr(), d->size());
                QBuffer device(&tmp);
                device.open(QIODevice::ReadOnly);
                validData.unite(adaptService( device, d.get(), dataHash ));
            }
        }
        // Send the data for adaption
        QByteArray tmp = QByteArray::fromRawData((char*)sd->ptr(), sd->size());
        QBuffer device(&tmp);
        device.open(QIODevice::ReadOnly);
        validData.unite(adaptStream( device, sd, dataHash ));

        static_cast<LockableStreamData*>(dataList[i].object())->served() = true;
    }

    return validData;
}

/**
 * @details
 * Initialises the direct stream data client, setting up the chunker manager
 * and checking that the data requested is available from the data manager.
 */
void DirectStreamDataClient::_init(QHash<QString, DataBlob*>& dataHash)
{
    // Initialise the chunker manager.
    _chunkerManager->init(*_dataManager);

    // Check that the hash of data blobs is complete.
    QSet<QString> reqs = _requireSet;
    if (!reqs.subtract(QSet<QString>::fromList(dataHash.keys())).isEmpty())
        throw QString("DirectStreamDataClient::getData(): Data hash does not "
                "contain objects for all possible requests.");

    // Check that the data requested is held by the data manager.
    _nPipelines = dataRequirements().size();
    QList<DataRequirements>::const_iterator it = dataRequirements().begin();
    while (it != dataRequirements().end())
    {
        if (!it->isCompatible( _dataManager->dataSpec() ))
            throw QString("DirectStreamDataClient::getData(): "
                    "Data requested not supported by client.");
        ++it;
    }
}

} // namespace pelican
