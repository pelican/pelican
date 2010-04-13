#ifndef DATATYPES_H
#define DATATYPES_H

#include <QString>
#include <QList>
#include "pelican/data/DataRequirements.h"
#include "pelican/adapters/AbstractAdapter.h"

/**
 * @file DataTypes.h
 */

namespace pelican {
    class AbstractStreamAdapter;
    class AbstractServiceAdapter;

/**
 * @class DataTypes
 *  
 * @brief
 *    Class to associate adapters with data types
 * @details
 * 
 */

class DataTypes
{
    public:
        DataTypes(  );
        ~DataTypes();

        /// add a data set
        void addData(const DataRequirements& data);
        void addData(const QList<DataRequirements>& data);

        /// set an adapter to interpret the data
        void setAdapter(const QString& type, AbstractAdapter* adapter);

        /// get a list of datarequirements. These will be adapted to be
        //  consistent with the adapters added with setAdapter
        const QList<DataRequirements>& dataRequirements() const;

        /// return a Stream Adapter for the specified type
        AbstractStreamAdapter* streamAdapter(const QString& type) const;

        /// return a Service Adapter for the specified type
        AbstractServiceAdapter* serviceAdapter(const QString& type) const;

        /// return the adapter type associated with the given data stream
        AbstractAdapter::AdapterType_t type(const QString& dataName) const;

    private:
        QList<DataRequirements> _dataRequirements; 
        QHash<QString,AbstractAdapter*> _adapters;
};

} // namespace pelican
#endif // DATATYPES_H 
