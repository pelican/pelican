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

#include "DataBlobVerify.h"
#include "data/DataBlob.h"
#include "data/DataBlobFactory.h"
#include <QtCore/QDebug>


namespace pelican {


/**
 *@details DataBlobVerify
 */
DataBlobVerify::DataBlobVerify( const DataBlob* blob )
    : _blob(blob)
{
    _buffer.open(QBuffer::WriteOnly);
}

/**
 *@details
 */
DataBlobVerify::~DataBlobVerify()
{
}

bool DataBlobVerify::verifySerialisedBytes() const {
    int size=_blob->serialisedBytes();
    _serialise();
    return size == _buffer.size();
}

bool DataBlobVerify::verifyDeserialise() const {
    _serialise();
    static FactoryGeneric<DataBlob> factory(true);
    DataBlob* copy = factory.create( _blob->type() );
    QByteArray tmp( _buffer.data() );
    QBuffer buffer2( &tmp );
    buffer2.open( QBuffer::ReadOnly );
    copy->deserialise( buffer2, QSysInfo::ByteOrder );
    QBuffer copyBuffer;
    copyBuffer.open(QBuffer::WriteOnly);
    copy->serialise( copyBuffer );
    if( copyBuffer.buffer().size() == 0 ) return false;
    return copyBuffer.buffer() == _buffer.buffer();
}

void DataBlobVerify::_serialise() const {
    if( _buffer.size() == 0 ) {
        _blob->serialise( _buffer );
    }
}

} // namespace pelican
