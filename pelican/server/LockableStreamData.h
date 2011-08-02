#ifndef LOCKABLESTREAMDATA_H
#define LOCKABLESTREAMDATA_H

#include <QtCore/QSet>
#include <QtCore/QString>
#include "pelican/server/AbstractLockableData.h"

/**
 * @file LockableStreamData.h
 */

namespace pelican {

class StreamData;
class LockedData;

/**
 * @ingroup c_server
 *
 * @class LockableStreamData
 *
 * @brief
 * Contains pointers to chunked LockableStream and a manifest of
 * associated data.
 *
 * @details
 * As well as a pointer to and the size of the data this class
 * also contains linking information to the service data
 */
class LockableStreamData : public AbstractLockableData
{
    Q_OBJECT

    public:
        typedef QList<LockedData> DataList_t;

    public:
        /// LockableStreamData constructor.
        LockableStreamData(const QString& name, void* memory = 0,
                size_t = 0, QObject* parent = 0);

        /// LockableStreamData destructor.
        ~LockableStreamData();

        StreamData* streamData() const;

        /// Test validity of all data.
        bool isValid() const;

        /// Returns true if the object has been served.
        bool& served() { return _served; };

        /// Test validity of data only taking into account the named associates.
        bool isValid(const QSet<QString>&) const;

        /// Associates a specific dataset with this LockableStreamData chunk.
        void addAssociatedData(const LockedData&);

        /// Returns the datasets associated with this LockableStreamData chunk.
        const DataList_t& associateData() const { return _serviceData;}
        const QSet<QString>& associateDataTypes() const { return _serviceDataTypes;}

        /// Reset the object's data and mark it of a suitable size
        void reset( size_t size );

    private:
        /// Disable copy constructor.
        LockableStreamData(const LockableStreamData&);

    private:
        DataList_t _serviceData;
        QSet<QString> _serviceDataTypes;
        bool _served;
};

} // namespace pelican

#endif // LOCKABLESTREAMDATA_H
