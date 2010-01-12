#ifndef STREAMDATA_H
#define STREAMDATA_H

#include <QSet>
#include <QString>
#include "Data.h"

/**
 * @file StreamData.h
 */

namespace pelican {

class LockedData;

/**
 * @class StreamData
 *  
 * @brief
 *     Contains Pointers to Chunked Stream and a manifest of associated data
 * @details
 *     As well as a pointer to and the size of the data this class
 *     also contains linking information to the service data
 */

class StreamData : public Data
{
    Q_OBJECT

    public:
        typedef QList<LockedData> DataList_t;

    public:
        StreamData(void* = 0, size_t  = 0);
        ~StreamData();
        void reset();

        /// test validity of all data
        bool isValid() const;

        /// test validity of data only taking into account the named associates
        bool isValid(const QSet<QString>&) const;

        /// associates a specific dataset with this StreamData chunk
        void addAssociatedData(const LockedData&);

        /// returns the datasets associated with this StreamData chunk
        const DataList_t& associateData() const;
        const QSet<QString>& associateDataTypes() const;

    private:
        StreamData(const StreamData&);

    private:
        DataList_t _serviceData;
        QSet<QString> _serviceDataTypes;
};

} // namespace pelican
#endif // STREAMDATA_H 
