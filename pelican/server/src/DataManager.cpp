#include "pelican/server/DataManager.h"

#include "pelican/server/LockableData.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/WritableData.h"
#include "pelican/comms/StreamData.h"
#include "pelican/utility/Config.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details
 * DataManager constructor.
 */
DataManager::DataManager(const Config* config)
{
    _config = config;
    _bufferConfigBaseAddress << Config::NodeId("server","");
    _bufferConfigBaseAddress << Config::NodeId("buffers","");
}


/**
 * @details
 * DataManager destructor.
 *
 * This deletes all the stream and service data buffers held by
 * the data manager.
 */
DataManager::~DataManager()
{
    foreach(StreamDataBuffer* s, _streams) {
        delete s;
    }
    foreach(ServiceDataBuffer* s, _service) {
        delete s;
    }
}


/**
 * @details
 *
 */
WritableData DataManager::getWritableData(const QString& type, size_t size)
{
    //std::cout << "pelican/dataManager::getWritableData, type : " << type.toStdString() << std::endl;
    if ( _streams.contains(type) ) {
        // XXX remove
        //std::cout << "pelican/dataManager::Returning writable stream data" << std::endl;
        return _streams[type]->getWritable(size);
    }
    if ( _service.contains(type) )
        return _service[type]->getWritable(size);
    return WritableData(0);
}


/**
 * @details
 * This method sets up a stream buffer for the specified data type
 * if it does not already exist.
 *
 * @param[in] type The data type held by the buffer.
 */
StreamDataBuffer* DataManager::getStreamBuffer(const QString& type)
{
    std::cout << "pelican/dataManager::getStreamBuffer" << type.toStdString() << std::endl;

    if ( ! _streams.contains(type) ) {
        Config::TreeAddress configAddress(_bufferConfigBaseAddress);
        configAddress << Config::NodeId(type, "");
        ConfigNode config = _config->get(configAddress);
        size_t maxSize = config.getOption("buffer", "maxSize", "10240").toUInt();
        size_t maxChunkSize = config.getOption("buffer", "maxChunkSize", "10240").toUInt();
        setStreamDataBuffer( type, new StreamDataBuffer(type, NULL, maxSize, maxChunkSize) );

    }
    return _streams[type];
}


/**
 * @details
 * This method sets up a service buffer for the specified data type
 * if it does not already exist.
 *
 * @param[in] type The data type held by the buffer.
 */
ServiceDataBuffer* DataManager::getServiceBuffer(const QString& type)
{
    if ( ! _service.contains(type) )
        setServiceDataBuffer( type, new ServiceDataBuffer(type) );
    return _service[type];
}


/**
 * @details
 * Note that the DataManager takes ownership of the StreamDataBuffer, and will
 * delete it on destruction.
 *
 */
void DataManager::setStreamDataBuffer(const QString& name, StreamDataBuffer* buffer)
{
    _specs.setStreamData(name);
    buffer->setDataManager(this);
    _streams[name]=buffer;
}


/**
 * @details
 * Note that the DataManager takes ownership of the ServiceDataBuffer, and will
 * delete it on destruction.
 */
void DataManager::setServiceDataBuffer(const QString& name, ServiceDataBuffer* buffer )
{
    _specs.setServiceData(name);
    _service[name]=buffer;
}


/**
 * @details
 *
 * TODO Please write a description of this!
 *
 * We make the assumption that this method will not be called with an
 * invalid type. No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type, const QSet<QString>& associateData )
{
    //std::cout << "pelican/dataManager::getNext() type = " << type.toStdString() << std::endl;
    LockedData lockedData = getNext(type);

    if( lockedData.isValid() ) {
        //std::cout << "pelican/dataManager::getNext(): Valid locked stream data found." << std::endl;

        // check it contains valid associateData
        LockableStreamData* streamData =
                static_cast<LockableStreamData*>(lockedData.object());
        QSet<QString> test = streamData->associateDataTypes();

        if(! ( associateData - test ).isEmpty() ) {
            return LockedData(0);
        }
    }
    return lockedData;
}


/**
 * @details
 *
 * TODO Please write a description of this!
 *
 * We make the assumption that this method will not be called with an
 * invalid type. No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type)
{
//     std::cout << "pelican/dataManager::getNext: type : " << type.toStdString() << std::endl;
    LockedData lockedData(type, 0);

    _streams[type]->getNext(lockedData);
    return lockedData;
}


/**
 * @details
 *         Attempt to fulfill a DataRequirement request for data
 * @return A list of locked data containing streams data objects (with the
 *         associated service data) or the request
 *         returns an empty string
 */
QList<LockedData> DataManager::getDataRequirements(const DataRequirements& req)
{
    QList<LockedData> dataList;
    if( ! req.isCompatible( dataSpec() ) )
        throw QString("Session::processStreamDataRequest(): "
                "pelican/data requested not supported by server");
    foreach (const QString stream, req.streamData() )
    {
        LockedData data = getNext(stream, req.serviceData());
        if( ! data.isValid() ) {
            dataList.clear();
            break; // one invalid stream invalidates the request
        }
        dataList.append(data);
    }
    return dataList;
}


/**
 * @details
 */
LockedData DataManager::getServiceData(const QString& type, const QString& version)
{
    LockedData lockedData(type,0);

    if( _service.contains(type) )
        _service[type]->getData(lockedData, version);
    return lockedData;
}


void DataManager::associateServiceData(LockableStreamData* data)
{
//     std::cout << "pelican/dataManager::associateServiceData()" << std::endl;
    QHashIterator<QString, ServiceDataBuffer*> i(_service);
    while (i.hasNext()) {
        i.next();
        LockedData d(i.key());
        i.value()->getCurrent(d);
        if( d.isValid() ) {
            data->addAssociatedData(d);
        }
    }
}


} // namespace pelican
