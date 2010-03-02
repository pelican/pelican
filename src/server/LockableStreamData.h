#ifndef LOCKABLESTREAMDATA_H
#define LOCKABLESTREAMDATA_H

#include <QSet>
#include <QString>
#include "LockableData.h"

/**
 * @file LockableStreamData.h
 */

namespace pelican {

class LockedData;

/**
 * @class LockableStreamData
 *  
 * @brief
 *     Contains Pointers to Chunked LockableStream and a manifest of associated data
 * @details
 *     As well as a pointer to and the size of the data this class
 *     also contains linking information to the service data
 */

class LockableStreamData : public LockableData
{
    public:
        typedef QList<LockedData> DataList_t;

    public:
        LockableStreamData(const QString& name, void* = 0, size_t  = 0);
        ~LockableStreamData();
        void reset();

        /// test validity of all data
        bool isValid() const;

        /// test validity of data only taking into account the named associates
        bool isValid(const QSet<QString>&) const;

        /// associates a specific dataset with this LockableStreamData chunk
        void addAssociatedData(const LockedData&);

        /// returns the datasets associated with this LockableStreamData chunk
        const DataList_t& associateData() const;
        const QSet<QString>& associateDataTypes() const;

    private:
        LockableStreamData(const LockableStreamData&);

    private:
        DataList_t _serviceData;
        QSet<QString> _serviceDataTypes;
};

} // namespace pelican
#endif // LOCKABLESTREAMDATA_H 
