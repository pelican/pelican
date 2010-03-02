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
    public:
        typedef QList<const Data*> DataList_t;

    public:
        StreamData(const QString& name, void* = 0, size_t  = 0);
        ~StreamData();

        void addAssociatedData(const Data*);
        const DataList_t& associateData() const;
        const QSet<QString>& associateDataTypes() const;
        bool isValid() const;
        void reset();

    private:
        StreamData(const StreamData&);

    private:
        DataList_t _associateData;
        QSet<QString> _associateDataTypes;
};

} // namespace pelican
#endif // STREAMDATA_H 
