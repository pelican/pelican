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

#include "server/LockableStreamData.h"
#include "server/LockableServiceData.h"
#include "server/LockedData.h"
#include "comms/DataChunk.h"
#include "comms/StreamData.h"

#include <iostream>

namespace pelican {

/**
 * @details
 * LockableStreamData constructor.
 */
LockableStreamData::LockableStreamData(const QString& name, void* memory,
        size_t size, QObject* parent) : AbstractLockableData(size, parent)
{
    _data.reset( new StreamData(name, memory, size) );
    _served = false;
}

LockableStreamData::~LockableStreamData()
{
}

void LockableStreamData::addAssociatedData(const LockedData& data)
{
    _serviceDataTypes.insert(data.name());
    _serviceData.append(data);
    LockableServiceData* lockable = static_cast<LockableServiceData*>(data.object());
    boost::shared_ptr<DataChunk> dataPtr( lockable->data() );
    streamData()->addAssociatedData( dataPtr );
}

bool LockableStreamData::isValid() const
{
    return isValid( _serviceDataTypes );
    /*
    bool rv = Data::isValid();
    foreach(LockedData data, _serviceData )
    {
        rv = rv && data.isValid();
    }
    return rv;
    */
}

bool LockableStreamData::isValid(const QSet<QString>& associates) const
{
    Q_ASSERT( data() != 0 );
    bool rv = data()->isValid();
    foreach(const QString& assoc, associates )
    {
        foreach(LockedData data, _serviceData )
        {
            if( data.name() == assoc )
                rv = rv && data.isValid();
        }
    }
    return rv;
}

void LockableStreamData::reset( size_t size )
{
    _serviceData.clear();
    _serviceDataTypes.clear();
    streamData()->reset( size );
}

StreamData* LockableStreamData::streamData() const {
    return static_cast<StreamData*>(_data.get());
}

} // namespace pelican
