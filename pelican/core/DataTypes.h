#ifndef DATATYPES_H
#define DATATYPES_H

/**
 * @file DataTypes.h
 */

#include "pelican/data/DataRequirements.h"
#include "pelican/core/AbstractAdapter.h"

#include <QtCore/QString>
#include <QtCore/QList>

namespace pelican {
class AbstractStreamAdapter;
class AbstractServiceAdapter;

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
        ~DataTypes();

        /// Add a data set.
        void addData(const DataRequirements& data);
        void addData(const QList<DataRequirements>& data);

        /// Set an adapter to interpret the data.
        void setAdapter(const QString& type, AbstractAdapter* adapter);

        /// Get a list of datarequirements. These will be adapted to be.
        //  consistent with the adapters added with setAdapter
        const QList<DataRequirements>& dataRequirements() const;

        /// Return a Stream Adapter for the specified type.
        AbstractStreamAdapter* streamAdapter(const QString& type) const;

        /// Return a Service Adapter for the specified type.
        AbstractServiceAdapter* serviceAdapter(const QString& type) const;

        /// Return the adapter type associated with the given data stream.
        AbstractAdapter::AdapterType_t type(const QString& dataName) const;

        /// return true is the object maps an adapter to the specified type
        bool adapterAvailable( const QString& type ) const;

    private:
        QList<DataRequirements> _dataRequirements;
        QHash<QString,AbstractAdapter*> _adapters;
};

} // namespace pelican
#endif // DATATYPES_H
