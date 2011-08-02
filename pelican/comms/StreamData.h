#ifndef STREAMDATA_H
#define STREAMDATA_H

/**
 * @file StreamData.h
 */

#include "pelican/comms/DataChunk.h"

#include <QtCore/QSet>
#include <QtCore/QString>
#include <boost/shared_ptr.hpp>

namespace pelican {

class LockedData;

/**
 * @ingroup c_comms
 *
 * @class StreamData
 *
 * @brief
 *     Contains Pointers to Chunked Stream and a manifest of associated data
 * @details
 *     As well as a pointer to and the size of the data this class
 *     also contains linking information to the service data
 */

class StreamData : public DataChunk
{
    public:
        typedef QList<boost::shared_ptr<DataChunk> > DataList_t;

    public:
        StreamData(const QString& name, void* = 0, size_t  = 0);
        StreamData(const QString& name, QString& id, size_t  = 0);
        StreamData(const QString& name, QString& id, QByteArray& d);
        virtual ~StreamData();

        void addAssociatedData(boost::shared_ptr<DataChunk>);
        const DataList_t& associateData() const;
        const QSet<QString>& associateDataTypes() const;
        virtual bool isValid() const;
        bool operator==(const StreamData& sd) const;
        void reset( size_t );

    private:
        StreamData(const StreamData&);

    private:
        DataList_t _associateData;
        QSet<QString> _associateDataTypes;
};

} // namespace pelican
#endif // STREAMDATA_H
