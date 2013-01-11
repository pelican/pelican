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

#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

/**
 * @file AbstractModule.h
 */

#include "utility/ConfigNode.h"
#include "utility/FactoryRegistrar.h"
#include "data/DataBlob.h"
#include <QtCore/QString>
#include <algorithm>
#include <iostream>
#include <vector>
using std::vector;

/// This macro is used to register the named module type.
#define PELICAN_DECLARE_MODULE(type) PELICAN_DECLARE(pelican::AbstractModule, type)

namespace pelican {
class AbstractPipeline;


/**
 * @ingroup c_core
 *
 * @class AbstractModule
 *
 * @brief
 * Abstract base class for all pipeline modules.
 *
 * @details
 * This is the base class for all Pelican pipeline modules, which provide
 * functionality to Pelican pipelines. Inherit this class to create a new
 * module type.
 */
class AbstractModule
{
    private:
        /// The configuration node for the module.
        ConfigNode _config;

        // reference to the pipeline the module is running in
        AbstractPipeline* _pipeline;

    public:
        /// Creates a new abstract Pelican module with the given configuration.
        PELICAN_CONSTRUCT_TYPES(ConfigNode)
        AbstractModule(const ConfigNode& config);

        /// Destroys the module (virtual).
        virtual ~AbstractModule();

        /// set the pipeline context
        void setPipeline( AbstractPipeline* p ) { _pipeline = p; };

        /// create a single DataBlob of the specified type
        DataBlob* createBlob(const QString& type) const;

        /**
         * @details
         * Sends data to the output streams managed by the OutputStreamManger
         *
         * @param[in] DataBlob to be sent.
         * @param[in] name of the output stream (defaults to DataBlob->type()).
         */
        void dataOutput( const DataBlob*, const QString& stream = "" ) const;

    protected:
        /// Returns the index of the first occurrence of value in the data.
        template <typename T>
        unsigned findIndex(T value, vector<T> const& data) const;

};




//------------------------------------------------------------------------------
// Method definitions.

template <typename T>
inline
unsigned AbstractModule::findIndex(T value, vector<T> const& data) const
{
    unsigned i;
    for (i = 0; i < data.size(); ++i) if (data[i] == value) break;

    if (i == data.size())
    {
        QString moduleName = _config.getDomElement().tagName();
        throw QString("%1: Index for selected value not found.").arg(moduleName);
    }
    return i;
}


} // namespace pelican

#endif // ABSTRACTMODULE_H
