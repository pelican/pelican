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

#include "comms/DataBlobResponse.h"
#include <QtCore/QDataStream>

namespace pelican {

/**
 * @details DataBlobResponse
 */
DataBlobResponse::DataBlobResponse(const QString& blobType,
        const QString& streamName,
        quint64 dataSize, QSysInfo::Endian endianness )
    : ServerResponse( ServerResponse::Blob ),
      _type(blobType),
      _name(streamName),
      _dataSize(dataSize),
      _endianness(endianness)
{
}

DataBlobResponse::DataBlobResponse( QDataStream& stream )
    : ServerResponse( ServerResponse::Blob )
{
    deserialise(stream);
}

/**
 * @details
 */
DataBlobResponse::~DataBlobResponse()
{
}

void DataBlobResponse::serialise( QDataStream& stream ) {
    stream <<  _type << _name;
    stream << _dataSize;
    stream << (int)_endianness;
}

void DataBlobResponse::deserialise( QDataStream& stream ) {
    stream >> _type >> _name;
    stream >> _dataSize;
    int tmp;
    stream >> tmp;
    _endianness = (QSysInfo::Endian)tmp;
}

size_t DataBlobResponse::serialisedSize() {
    return sizeof(_type) + sizeof(_name) + sizeof(_dataSize) + sizeof(_endianness);
}
} // namespace pelican
