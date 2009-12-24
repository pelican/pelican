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
    public:
        StreamData(void* = 0, size_t  = 0);
        ~StreamData();
        void reset();
        bool isValid() const;
        void addAssociatedData(LockedData);
        QList<LockedData>& associateData();

    private:
        StreamData(const StreamData&);

    private:
        QList<LockedData> _serviceData;
};

} // namespace pelican
#endif // STREAMDATA_H 
