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

#ifndef ABSTRACTSERVICEADAPTER_H
#define ABSTRACTSERVICEADAPTER_H

/**
 * @file AbstractServiceAdapter.h
 */

#include "core/AbstractAdapter.h"

namespace pelican {

class ConfigNode;
class DataBlob;

/**
 * @ingroup c_core
 *
 * @class AbstractServiceAdapter
 * @deprecated
 *
 * @brief
 * Abstract base class for all service data adapters.
 *
 * @details
 * This class is the base class for all Pelican service data adapters, which
 * de-serialise data from an input stream.
 *
 * Normally you should inherit directly form AbstractAdapter unless you wish to
 * force pelican to ignore the server configuration and never provide the
 * dependent data
 *
 * Inherit this class and implement the deserialise() method to create a new
 * adapter.
 */
class AbstractServiceAdapter : public AbstractAdapter
{
    public:
        /// Constructs a new service data adapter with the given configuration.
        AbstractServiceAdapter(const ConfigNode& config)
        : AbstractAdapter(config, AbstractAdapter::Service) {}

        /// Destroys the service adapter (virtual).
        virtual ~AbstractServiceAdapter() {}

};

} // namespace pelican

#endif // ABSTRACTSERVICEADAPTER_H
