#include <QBuffer>
#include "DirectStreamDataClient.h"
#include "comms/StreamData.h"
#include "data/DataRequirements.h"
#include "server/LockedData.h"
#include "server/LockableStreamData.h"


#include "utility/memCheck.h"

namespace pelican {
class ConfigNode;
class DataTypes;


/**
 *@details DirectStreamDataClient 
 */
DirectStreamDataClient::DirectStreamDataClient(const ConfigNode& config, const DataTypes& types)
    : AbstractDataClient(config, types)
{
    _started = false;
}

/**
 *@details
 */
DirectStreamDataClient::~DirectStreamDataClient()
{
}

void DirectStreamDataClient::setChunker(AbstractChunker* chunker, const QString& name)
{
    switch( type(name) ) {
        case( AbstractAdapter::Stream ) :
            _chunkerManager.addStreamChunker(chunker);
            break;
        case( AbstractAdapter::Service ) :
            _chunkerManager.addServiceChunker(chunker);
            break;
        default:
            throw(QString( QString("data type ") + name + " not available in requirements"));
            break;
    }

}

void DirectStreamDataClient::start()
{
    if( ! _started ) {
        _started = true;
        _chunkerManager.init(_dataManager);
    }
}

QHash<QString, DataBlob*> DirectStreamDataClient::getData(QHash<QString, DataBlob*>& dataHash)
{
    QHash<QString, DataBlob*> validData;
    QSet<QString> reqs = _requireSet;
    if( ! reqs.subtract(QSet<QString>::fromList(dataHash.keys())).isEmpty() )
        throw(QString("DirectStreamDataClient::getData() data hash does not "
                "contain objects for all possible requests"));
    Q_ASSERT( _started ); // ensure somebody has not forgotten to start

    // keep polling the data manager until we can match a suitable request
    QList<LockedData> dataList; // will contain the list of valid StreamDataObjects
    while( dataList.size() == 0 ) {
        QList<DataRequirements>::const_iterator it = dataRequirements().begin();
        while( it != dataRequirements().end() && dataList.size() == 0 )
        {
            if( ! it->isCompatible( _dataManager.dataSpec() ) )
                throw QString("DirectStreamDataClient::getData(): "
                        "Data requested not supported by client");
            dataList = _dataManager.getDataRequirements(*it);
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
                validData.unite(adaptService( device, d.get(), dataHash ));
            }
        }
        // send the data for adaption
        QByteArray tmp_array = QByteArray::fromRawData((char*)sd->operator*(), sd->size());
        QBuffer device(&tmp_array);
        validData.unite(adaptStream( device, sd, dataHash ));
    }

    return validData;
}


} // namespace pelican
