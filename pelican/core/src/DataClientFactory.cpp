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

#include "core/DataClientFactory.h"
#include "core/FileDataClient.h"
#include "core/PelicanServerClient.h"
#include "core/DataTypes.h"
#include "data/DataRequirements.h"

#include <iostream>

namespace pelican {

/**
 * @details
 * Method to create a pointer to a data client of the specified \p type.
 *
 * @param[in] type          The type of data client object to create.
 * @param[in] requirements  The data sets to be provided by the client.
 * @param[in] name          An optional specific named configuration to use.
 */
AbstractDataClient* DataClientFactory::create(const QString& type, const QString& name)
{
    // Create a DataTypes object from the DataRequirements.
    DataTypes dataTypes( conf(type,name), _adapterFactory );

    // Call the base class implementation and set the data requirements.
    AbstractDataClient* client = FactoryConfig<AbstractDataClient>::create(
            type, dataTypes, _config , name);

    return client;
}

} // namespace pelican
