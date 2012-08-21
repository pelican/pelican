#include "pelican/server/DataManager.h"

#include "pelican/server/LockableServiceData.h"
#include "pelican/server/LockableStreamData.h"
#include "pelican/server/WritableData.h"
#include "pelican/comms/StreamData.h"
#include "pelican/utility/Config.h"

#include <iostream>
#include <QtCore/QMutexLocker>

namespace pelican {


/**
 * @details
 * DataManager constructor.
 */
DataManager::DataManager(const Config* config, const QString section)
    : _config(config), _verboseLevel(0)
{
    _bufferConfigBaseAddress << Config::NodeId(section,"");
    _bufferConfigBaseAddress << Config::NodeId("buffers","");
}

DataManager::DataManager(const Config* config, const Config::TreeAddress& base)
    : _config(config)
{
    _bufferConfigBaseAddress = base;
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


void DataManager::verbose( const QString& msg, int verboseLevel )
{
    if( verboseLevel <= _verboseLevel )
        std::cout << "DataManager: "
                  << msg.toStdString()
                  << std::endl;
}
/**
 * @details
 *
 */
WritableData DataManager::getWritableData(const QString& type, size_t size)
{
    verbose("data block of type \"" + type + "\" requested (size=" + QString().setNum((int)size) + ")", 2 );
    if ( _streams.contains(type) ) {
        verbose( "returning stream", 2);
        return _streams[type]->getWritable(size);
    }

    if ( _service.contains(type) )
        return _service[type]->getWritable(size);

    verbose("data block of type \"" + type + "\" unknown", 2 );
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
    Q_ASSERT( type != "" );

    if ( ! _streams.contains(type) ) {
        Config::TreeAddress configAddress(_bufferConfigBaseAddress);
        configAddress << Config::NodeId(type, "");
        ConfigNode config = _config->get(configAddress);
        if( ! _bufferMaxSizes.contains(type) ) {
           _bufferMaxSizes[type] = config.getOption("buffer", "maxSize", "10240").toULongLong();
        }
        if( ! _bufferMaxChunkSizes.contains(type) ) {
            _bufferMaxChunkSizes[type] = config.getOption("buffer", "maxChunkSize", "10240").toULongLong();
        }
        setStreamDataBuffer( type, new StreamDataBuffer(type, _bufferMaxSizes[type], _bufferMaxChunkSizes[type]) );
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
    verbose("Adding StreamBuffer \"" + name + "\" of size " + QString().setNum( buffer->size() ));
    _specs.setStreamData(name);
    buffer->setVerbosity(_verboseLevel);
    buffer->setDataManager(this);
    _streams[name]=buffer;
}

void DataManager::setMaxBufferSize( const QString& stream, size_t size ) {
    _bufferMaxSizes[stream] = size;
}

void DataManager::setMaxChunkSize( const QString& stream, size_t size ) {
     _bufferMaxChunkSizes[stream] = size;
}

/**
 * @details
 * Note that the DataManager takes ownership of the ServiceDataBuffer, and will
 * delete it on destruction.
 */
void DataManager::setServiceDataBuffer(const QString& name, ServiceDataBuffer* buffer )
{
    verbose("Adding ServiceBuffer \"" + name + "\"");
    buffer->setVerbosity(_verboseLevel);
    _specs.setServiceData(name);
    _service[name]=buffer;
}


/**
 * @details
 *
 * We make the assumption that this method will not be called with an
 * invalid type. No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type, const QSet<QString>& associateData )
{
    LockedData lockedData = getNext(type);

    if( lockedData.isValid() )
    {
        // check it contains valid associateData
        LockableStreamData* streamData =
                static_cast<LockableStreamData*>(lockedData.object());
        QSet<QString> test = streamData->associateDataTypes();

#ifdef BROKEN_QT_SET_HEADER
        QSet<QString> temp = associateData;
        if(! ( temp - test ).isEmpty() ) {
#else
        if(! ( associateData - test ).isEmpty() ) {
#endif // BROKEN_QT_SET_HEADER
            return LockedData(0);
        }
    }
    return lockedData;
}


/**
 * @details
 *
 * We make the assumption that this method will not be called with an
 * invalid type.
 *
 * WARNING: No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type)
{
    LockedData lockedData(type, 0);

    verbose("getNext(" + type + ") called", 2 );
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
    if( ! req.isCompatible( dataSpec() ) ) {
        QString msg("Session::processStreamDataRequest(): "
                "data requested not supported by server");
        verbose(msg);
        throw msg;
    }
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


/**
 * @details
 */
void DataManager::associateServiceData(LockableStreamData* data)
{
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

void DataManager::deactivateStream(const QString& stream)
{
    // if it is streaming data we simply wait untill 
    // the buffer is empty
    if( _streams.contains(stream) ) {
        _deactivate[_streams[stream]] = stream; 
    }
}

void DataManager::emptiedBuffer(StreamDataBuffer* buffer) 
{
    if( _deactivate.contains(buffer) ) {
        // remove from the data specifications
        QString name=_deactivate[buffer];
        _specs.removeStreamData(name);
        // remove the buffer
        _streams.remove(name);
        _deactivate.remove(buffer);
        delete buffer;
    }
}


} // namespace pelican
