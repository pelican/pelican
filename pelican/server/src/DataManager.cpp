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

#include "utility/Config.h"

#include "server/LockableServiceData.h"
#include "server/LockableStreamData.h"
#include "server/StreamDataBuffer.h"
#include "server/ServiceDataBuffer.h"
#include "server/WritableData.h"
#include "comms/StreamData.h"

#include <QtCore/QMutexLocker>
#include <QtCore/QDebug>

#include <iostream>

namespace pelican {


/**
 * @details
 * Constructor
 */
DataManager::DataManager(const Config* config, const QString section)
: _config(config), _verboseLevel(0)
{
    _bufferConfigBaseAddress << Config::NodeId(section, "");
    _bufferConfigBaseAddress << Config::NodeId("buffers", "");
}


/**
 * @details
 * Constructor
 */
DataManager::DataManager(const Config* config, const Config::TreeAddress& base)
: _config(config), _verboseLevel(0)
{
    _bufferConfigBaseAddress = base;
}


/**
 * @details
 * Destructor.
 *
 * Deletes all the stream and service data buffers held by the data manager.
 */
DataManager::~DataManager()
{
    foreach (StreamDataBuffer* s, _streams) {
        delete s;
    }
    foreach (ServiceDataBuffer* s, _service) {
        delete s;
    }
}


/**
 * @details
 */
void DataManager::addDefaultAdapters(const QHash<QString,QString>& types)
{
    _specs.addAdapterTypes(types);
}


/**
 * @details
 */
void DataManager::associateServiceData(LockableStreamData* data)
{
    QHashIterator<QString, ServiceDataBuffer*> i(_service);
    while (i.hasNext())
    {
        i.next();
        LockedData d(i.key());
        i.value()->getCurrent(d);
        if (d.isValid()) {
            data->addAssociatedData(d);
        }
    }
}

/**
 * @details
 * Marks the specified stream for deactivation. If still streaming we wait
 * until the buffer is empty
 *
 * NOTE unused and untested!
 *
 * @param stream Type (name) of stream to deactivate.
 */
void DataManager::deactivateStream(const QString& stream)
{
    // If it is streaming data we simply wait until the buffer is empty
    if (_streams.contains(stream)) {
        _deactivate[_streams[stream]] = stream;
    }
}


/**
 * @details
 *
 * @param buffer
 */
void DataManager::emptiedBuffer(StreamDataBuffer* buffer)
{
    if( _deactivate.contains(buffer) ) {
        // Remove from the data specifications
        QString name=_deactivate[buffer];
        _specs.removeStreamData(name);
        // Remove the buffer
        _streams.remove(name);
        _deactivate.remove(buffer);
        delete buffer;
    }
}


/**
 * @details
 * Attempt to fulfil a DataRequirement request for data
 *
 * @return A list of locked data containing streams data objects (with the
 * associated service data) or the request
 * returns an empty string
 */
QList<LockedData> DataManager::getDataRequirements(const DataSpec& req)
{
    QList<LockedData> dataList;
    if (!req.isCompatible(dataSpec())) {
        QString msg("Session::processStreamDataRequest(): Data requested not"
                " supported by server");
        verbose(msg);
        throw msg;
    }
    foreach (const QString stream, req.streamData())
    {
        LockedData data = getNext(stream, req.serviceData());
        // One invalid stream invalidates the request.
        if (!data.isValid()) {
            dataList.clear();
            break;
        }
        dataList.append(data);
    }
    return dataList;
}


/**
 * @details
 * We make the assumption that this method will not be called with an
 * invalid type. No checking in order to speed things up.
 */
LockedData DataManager::getNext(const QString& type,
        const QSet<QString>& associateData)
{
    LockedData lockedData = getNext(type);

    if (lockedData.isValid())
    {
        // check it contains valid associateData
        LockableStreamData* streamData =
                static_cast<LockableStreamData*>(lockedData.object());
        QSet<QString> test = streamData->associateDataTypes();

#ifdef BROKEN_QT_SET_HEADER
        QSet<QString> temp = associateData;
        if (!(temp - test).isEmpty()) {
#else
        if (!(associateData - test).isEmpty()) {
#endif // BROKEN_QT_SET_HEADER
            return LockedData(0);
        }
    }
    return lockedData;
}


/**
 * @details
 * Returns the next data block from Stream Data.
 *
 * We make the assumption that this method will not be called with an invalid
 * type.
 *
 * WARNING: No checking in order to speed things up.
 *
 * @param[in] type The type (name) of the data requested
 *
 * @return
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
            _bufferMaxSizes[type] = maxSize;

        if (!_bufferMaxChunkSizes.contains(type)) {
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
 */
LockedData DataManager::getServiceData(const QString& type, const QString& version)
{
    LockedData lockedData(type, 0);

    if (_service.contains(type))
        _service[type]->getData(lockedData, version);
    return lockedData;
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
    Q_ASSERT(type != "");
    if (!_streams.contains(type))
    {
        Config::TreeAddress configAddress(_bufferConfigBaseAddress);
        configAddress << Config::NodeId(type, "");
        ConfigNode config = _config->get(configAddress);
        if (!_bufferMaxSizes.contains(type))
        {
            _bufferMaxSizes[type] = config.getOption("buffer", "maxSize",
                    "10240").toULongLong();
        }
        if (!_bufferMaxChunkSizes.contains(type))
        {
            _bufferMaxChunkSizes[type] = config.getOption("buffer",
                    "maxChunkSize", 0).toULongLong();
            if (!_bufferMaxChunkSizes[type]) {
                _bufferMaxChunkSizes[type]=_bufferMaxSizes[type];
            }
        }
        setStreamDataBuffer(type, new StreamDataBuffer(type,
                _bufferMaxSizes[type], _bufferMaxChunkSizes[type]));
    }
    return _streams[type];
}


/**
 * @details
 */
WritableData DataManager::getWritableData(const QString& type, size_t size)
{
    verbose("data block of type \"" + type + "\" requested (size=" +
            QString().setNum((int)size) + ")", 2);

    if (_streams.contains(type)) {
        verbose( "returning stream", 2);
        return _streams[type]->getWritable(size);
    }

    if (_service.contains(type))
        return _service[type]->getWritable(size);

    verbose("data block of type \"" + type + "\" unknown", 2 );
    return WritableData(0);
}

/**
 * @details
 * NOTE only used in a ChunkerTester - remove?
 */
void DataManager::setMaxBufferSize(const QString& stream, size_t size) {
    _bufferMaxSizes[stream] = size;
}


/**
 * @details
 * NOTE only used in a ChunkerTester - remove?
 */
void DataManager::setMaxChunkSize(const QString& stream, size_t size) {
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
 * Note that the DataManager takes ownership of the StreamDataBuffer, and will
 * delete it on destruction.
 */
void DataManager::setStreamDataBuffer(const QString& name, StreamDataBuffer* buffer)
{
    verbose("Adding StreamBuffer \"" + name + "\" of size " + QString().setNum(buffer->maxSize()));
    _specs.addStreamData(name);
    buffer->setVerbosity(_verboseLevel);
    buffer->setDataManager(this);
    _streams[name]=buffer;
}


int DataManager::numBuffers() const
{
    int num = _streams.size() + _service.size();
    Q_ASSERT(num == _specs.allData().size());
    return num;
}
int DataManager::numStreamBuffers() const
{
    int num = _streams.size();
    Q_ASSERT(num == _specs.streamData().size());
    return num;
}
int DataManager::numServiceBuffers() const
{
    int num = _service.size();
    Q_ASSERT(num == _specs.serviceData().size());
    return num;
}
int DataManager::numDeactivatedBuffers() const {
    return _deactivate.size();
}
bool DataManager::isStream(const QString& type) const
{
    bool ans = _streams.contains(type);
    Q_ASSERT(ans == _specs.streamData().contains(type));
    return ans;
}
bool DataManager::isService(const QString& type) const
{
    bool ans = _service.contains(type);
    Q_ASSERT(ans == _specs.serviceData().contains(type));
    return ans;
}
size_t DataManager::maxSize(const QString& type) const
{
    Q_ASSERT(_bufferMaxSizes.contains(type));
    return _bufferMaxSizes[type];
}
size_t DataManager::maxChunkSize(const QString& type) const
{
    Q_ASSERT(_bufferMaxChunkSizes.contains(type));
    return _bufferMaxChunkSizes[type];
}
size_t DataManager::allocatedSize(const QString& type) const
{
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    if (_streams.contains(type))
    {
        const StreamDataBuffer* buf = _streams[type];
        Q_ASSERT(buf->maxSize() == maxSize(type));
        return (buf->maxSize() - buf->space());
    }
    else if (_service.contains(type))
    {
        const ServiceDataBuffer* buf = _service[type];
        Q_ASSERT(buf->maxSize() == maxSize(type));
        return (buf->maxSize() - buf->space());
    }
    return 0;
}
size_t DataManager::usableSize(const QString& type, size_t chunkSize) const
{
    // FIXME require that chunkSize != 0? need to change buffer functions if so...
    // Currently chunkSize == 0 implies all space and the full size of
    // empty/expired can be used. Not sure what the best solution is here...
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    if (_streams.contains(type))
    {
        return _streams[type]->usableSize(chunkSize);
    }
    else if (_service.contains(type))
    {
        return _service[type]->usableSize(chunkSize);
    }
    return 0;
}
size_t DataManager::usedSize(const QString& type) const
{
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    if (_streams.contains(type))
    {
        return _streams[type]->usedSize();
    }
    else if (_service.contains(type))
    {
        return _service[type]->usedSize();
    }
    return 0;
}
int DataManager::numChunks(const QString& type) const
{
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    if (_streams.contains(type))
    {
        const StreamDataBuffer* buf = _streams[type];
        return buf->numChunks();
    }
    else if (_service.contains(type))
    {
        const ServiceDataBuffer* buf = _service[type];
        return buf->numChunks();
    }
    return 0;
}
int DataManager::numActiveChunks(const QString& type) const
{
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    if (_streams.contains(type))
    {
        const StreamDataBuffer* buf = _streams[type];
        return buf->numberOfActiveChunks();
    }
    else if (_service.contains(type))
    {
        const ServiceDataBuffer* buf = _service[type];
        // Service data chunks are active unless expired.
        return buf->numChunks() - buf->numEmptyChunks();
    }
    return 0;
}
int DataManager::numExpiredChunks(const QString& type) const
{
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    if (_streams.contains(type))
    {
        const StreamDataBuffer* buf = _streams[type];
        return buf->numberOfEmptyChunks();
    }
    else if (_service.contains(type))
    {
        const ServiceDataBuffer* buf = _service[type];
        return buf->numEmptyChunks();
    }
    return 0;
}

int DataManager::numUsableChunks(const QString& type, size_t chunkSize) const
{
    Q_ASSERT(_streams.contains(type) || _service.contains(type));
    Q_ASSERT(chunkSize > 0);
    if (_streams.contains(type))
    {
        //const StreamDataBuffer* buf = _streams[type];
        StreamDataBuffer* buf = _streams[type];
        return buf->numUsableChunks(chunkSize);
    }
    else if (_service.contains(type))
    {
        ServiceDataBuffer* buf = _service[type];
        return buf->numUsableChunks(chunkSize);
    }
    return 0;
}


void DataManager::verbose(const QString& msg, int verboseLevel)
{
    if (verboseLevel <= _verboseLevel)
        std::cout << "DataManager: " << msg.toStdString() << std::endl;
}



} // namespace pelican
