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

#include "DataBlobBuffer.h"
#include "data/DataBlob.h"
#include <iostream>

namespace pelican {

/**
 *@details DataBlobBuffer
 */
DataBlobBuffer::DataBlobBuffer()
: _index(-1), _size(0)
{
}

/**
 *@details
 */
DataBlobBuffer::~DataBlobBuffer()
{
     foreach(DataBlob* blob, _data) {
        delete blob;
     }
}

void DataBlobBuffer::addDataBlob(DataBlob* blob)
{
     _data.append(blob);
     _size = _data.size();
}

DataBlob* DataBlobBuffer::next() {
    //_index=++_index%_size; // FIXME this line is a bit dodgy
    _index = (_index + 1) % _size; // NOTE this replacement for the line above needs checking.
    return _data[_index];
}

void DataBlobBuffer::shrink(int newSize) {
    // remove oldest/unused data first
    while( _data.size() > newSize ) {
        unsigned int index = (_index+1) % _size;
        delete _data[index];
        _data.removeAt(index);
        if( index < _index && _index != (unsigned int)-1 ) { --_index; }
    }
    _size=_data.size();
}

} // namespace pelican
