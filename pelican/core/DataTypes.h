#ifndef DATATYPES_H
#define DATATYPES_H

/**
 * @file DataTypes.h
 */

#include "pelican/data/DataSpec.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/core/AbstractAdapter.h"

#include <QtCore/QString>
#include <QtCore/QList>

namespace pelican {
class AbstractStreamAdapter;
class AbstractServiceAdapter;
class AbstractAdapterFactory;

/**
 * @ingroup c_core
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

        /// Add a data set.
        void addData(const DataSpec& data);
        void addData(const QList<DataSpec>& data);

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
        AbstractAdapter* _createAdapter( const QString& type ) const;

    private:
        QList<DataSpec> _dataRequirements;
        QHash<QString,AbstractAdapter*> _adapters;
        ConfigNode      _conf;
        AbstractAdapterFactory* _adapterFactory;
        QHash<QString, QString> _adapterNames;
};

} // namespace pelican
#endif // DATATYPES_H
