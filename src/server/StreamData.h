#ifndef STREAMDATA_H
#define STREAMDATA_H

#include <QList>
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
        bool isValid() const;

        /// associates a specific dataset with this StreamData chunk
        void addAssociatedData(const LockedData&);
        void addAssociatedData(Data*);

        /// returns the datasets associated with this StreamData chunk
        DataList_t& associateData();

    private:
        StreamData(const StreamData&);

    private:
        DataList_t _serviceData;
};

} // namespace pelican
#endif // STREAMDATA_H 
