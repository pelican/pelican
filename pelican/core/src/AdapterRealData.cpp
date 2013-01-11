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

#include "core/AdapterRealData.h"
#include "data/ArrayData.h"
#include "utility/ConfigNode.h"
#include <QtCore/QIODevice>

namespace pelican {

/**
 * @details
 * Constructs a stream adapter for a real-valued data set.
 * Extracts the data blob dimensions from the configuration node,
 * setting some defaults if configuration options can't be found.
 *
 * @param[in]   config  Pelican XML configuration node object.
 */
AdapterRealData::AdapterRealData(const ConfigNode& config)
: AbstractStreamAdapter(config)
{
}

/**
 * @details
 * Method to deserialise a real-valued data chunk
 * contained in a QIODevice into a Pelican real-valued data blob.
 *
 * @param[in] in  Pointer to a QIODevice containing serialised data.
 */
void AdapterRealData::deserialise(QIODevice* in)
{
    // Set the size of the data blob to fill.
    DoubleData* blob = static_cast<DoubleData*>(_data);
    unsigned length = _chunkSize / sizeof(double);
    if (_chunkSize % sizeof(double) != 0) length++;
    blob->resize(length);

    // Read the data off the device.
    in->read((char*)blob->ptr(), _chunkSize);
}

} // namespace pelican
