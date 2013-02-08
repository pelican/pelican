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

#include "comms/StreamData.h"
#include "comms/DataChunk.h"
#include <iostream>

namespace pelican {

// class StreamData
StreamData::StreamData(const QString& name, void* data, size_t size)
    : DataChunk(name, data, size)
{
}

StreamData::StreamData(const QString& name, QString& id, QByteArray& d )
    : DataChunk(name, id, d)
{
}

StreamData::StreamData(const QString& name, QString& id, size_t size)
    : DataChunk(name, id, size)
{
}

StreamData::~StreamData()
{
}

const StreamData::DataList_t& StreamData::associateData() const
{
    return _associateData;
}

const QSet<QString>& StreamData::associateDataTypes() const
{
    return _associateDataTypes;
}

void StreamData::addAssociatedData(boost::shared_ptr<DataChunk> data)
{
    Q_ASSERT( data.get() );
    _associateData.append(data);
    _associateDataTypes.insert(data->name());
}

bool StreamData::isValid() const
{
    bool rv = DataChunk::isValid();
    if( rv ) {
        foreach(const boost::shared_ptr<DataChunk>& d, _associateData) {
            rv &= d->isValid();
        }
    }
    return rv;
}

bool StreamData::operator==(const StreamData& sd) const
{
    if( _associateData.size() != sd._associateData.size() )
        return false;
    bool rv = DataChunk::operator==(sd);
    for( int i = 0; i < _associateData.size(); ++i )
    {
        rv &= _associateData[i]->operator==(*(sd._associateData[i]));
    }
    return rv;
}


void StreamData::reset( size_t size )
{
    _associateData.clear();
    setSize(size);
}

} // namespace pelican
