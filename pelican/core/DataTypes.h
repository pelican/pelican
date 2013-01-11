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

#ifndef DATATYPES_H
#define DATATYPES_H

/**
 * @file DataTypes.h
 */

#include "data/DataSpec.h"
#include "utility/ConfigNode.h"
#include "core/AbstractAdapter.h"

#include <QtCore/QString>
#include <QtCore/QList>

namespace pelican {
class AbstractStreamAdapter;
class AbstractServiceAdapter;
class AbstractAdapterFactory;

/** * @ingroup c_core
 *
 * @class DataTypes
 *
 * @brief
 * Class to associate adapters with data types
 *
 * @details
 */

class DataTypes
{
    public:
        DataTypes();
        DataTypes( const ConfigNode& config, AbstractAdapterFactory* );
        ~DataTypes();

        /// Add a data set, and attempt to find a suitable adapter.
        /// Will throw QString if no suitable adapter is found
        void addData(const DataSpec& data );
        void addData(const QList<DataSpec>& data );

        /// Set an adapter to interpret the data.
        void setAdapter(const QString& type, AbstractAdapter* adapter);

        /// Get a list of datarequirements. These will be adapted to be.
        //  consistent with the adapters added with setAdapter
        const QList<DataSpec>& dataSpec() const;

        /// Return a Stream Adapter for the specified type.
        AbstractStreamAdapter* streamAdapter(const QString& type) const;

        /// Return a Service Adapter for the specified type.
        AbstractServiceAdapter* serviceAdapter(const QString& type) const;

        /// Return the adapter type associated with the given data stream.
        AbstractAdapter::AdapterType_t type(const QString& dataName) const;

        /// return true is the object maps an adapter to the specified type
        bool adapterAvailable( const QString& type ) const;

    private:
        AbstractAdapter* _createAdapter( const QString& type,
                const AbstractAdapter::AdapterType_t& ) const;
        void _setAdapter( DataSpec& req, const QString& type );
        void _addData( const QSet<QString>& data, const QHash<QString,QString>& defaultAdapters,
                       const AbstractAdapter::AdapterType_t& defaultType );

    private:
        QList<DataSpec> _dataRequirements;
        QHash<QString,AbstractAdapter*> _adapters;
        ConfigNode      _conf;
        AbstractAdapterFactory* _adapterFactory;
        QHash<QString, QString> _adapterNames;
};

} // namespace pelican
#endif // DATATYPES_H
