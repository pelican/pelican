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

#ifndef ADAPTERREALDATA_H
#define ADAPTERREALDATA_H

/**
 * @file AdapterRealData.h
 */

#include "core/AbstractStreamAdapter.h"

namespace pelican {

class ConfigNode;

/**
 * @ingroup c_core
 *
 * @class AdapterRealData
 *
 * @brief
 * Adapter to deserialise a chunk of data into a PELICAN real-valued data blob.
 *
 * @details
 * Stream adapter to deserialise a chunk of data into a PELICAN real-valued
 * data blob. The initial size of the visibility data blob is set from the
 * configuration options.
 */
class AdapterRealData : public AbstractStreamAdapter
{
        friend class AdapterRealDataTest;

    public:
        /// Constructs the adapter
        AdapterRealData(const ConfigNode& config);

    protected:
        /// Method to deserialise memory held by an I/O device.
        void deserialise(QIODevice* in);
};

PELICAN_DECLARE_ADAPTER(AdapterRealData)

} // namespace pelican

#endif // ADAPTERREALDATA_H
