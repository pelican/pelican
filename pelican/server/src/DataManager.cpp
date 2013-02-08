/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "server/DataManager.h"

#include "server/LockableServiceData.h"
#include "server/LockableStreamData.h"
#include "server/WritableData.h"
#include "comms/StreamData.h"
#include "utility/Config.h"

#include <iostream>
#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>

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
    : _config(config), _verboseLevel(0)
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
    verbose("data block of type \"" + type + "\" requested (size=" +
            QString().setNum((int)size) + ")", 2);

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
            _bufferMaxChunkSizes[type] = config.getOption("buffer", "maxChunkSize", 0).toULongLong();
            if( ! _bufferMaxChunkSizes[type] ) _bufferMaxChunkSizes[type]=_bufferMaxSizes[type];
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
    if (!_service.contains(type))
    {
        Config::TreeAddress configAddress(_bufferConfigBaseAddress);
        configAddress << Config::NodeId(type, "");
        ConfigNode config = _config->get(configAddress);

        qulonglong maxSize = config.getOption("buffer", "maxSize", "10240").toULongLong();
        qulonglong maxChunkSize = config.getOption("buffer", "maxChunkSize", 0).toULongLong();

        if (!_bufferMaxSizes.contains(type))
        {
            _bufferMaxSizes[type] = maxSize;
        }
        if (!_bufferMaxChunkSizes.contains(type))
        {
            _bufferMaxChunkSizes[type] = maxChunkSize;
            if (!_bufferMaxChunkSizes[type])
                _bufferMaxChunkSizes[type] = _bufferMaxSizes[type];
        }

        setServiceDataBuffer(type, new ServiceDataBuffer(type,
                _bufferMaxSizes[type], _bufferMaxChunkSizes[type]));
    }
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
    _specs.addStreamData(name);
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
    _specs.addServiceData(name);
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
QList<LockedData> DataManager::getDataRequirements(const DataSpec& req)
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

void DataManager::addDefaultAdapters( const QHash<QString,QString>& types ) {
    _specs.addAdapterTypes( types );
}

} // namespace pelican
