#ifndef SERVICEDATABUFFER_H
#define SERVICEDATABUFFER_H

#include "pelican/server/AbstractDataBuffer.h"

#include <QtCore/QObject>
#include <QtCore/QHash>

/**
 * @file ServiceDataBuffer.h
 */

namespace pelican {

class LockableServiceData;
class LockedData;

/**
 * @ingroup c_server
 *
 * @class ServiceDataBuffer
 *
 * @brief
 * Maintains service data.
 *
 * @details
 * Access to service data is assumed to be by direct reference,
 * either through an explicit identifier, or through the current()
 * method.
 * Multiple threads may access the same data at the same time for
 * reading.
 */
class ServiceDataBuffer : public AbstractDataBuffer
{
    Q_OBJECT

    public:
        /// Constructs a service data buffer.
        ServiceDataBuffer(const QString& type,
                const size_t max = 10240,
                const size_t maxChunkSize = 10240,
                QObject* parent = 0);

        /// Destroys the service data buffer.
        ~ServiceDataBuffer();

        /// Return the specified version of the service data.
        void getData(LockedData& lockedData, const QString& version);

        /// Return the current version of the service data.
        void getCurrent(LockedData& lockedData);

        /// Returns a section of writable memory to be filled.
        WritableData getWritable(size_t size);

    protected slots:
        void activateData();
        void deactivateData();

    protected:
        void activateData(LockableServiceData*);
        void deactivateData(LockableServiceData*);

    private:
        QHash<QString, LockableServiceData*> _data;
        QList<LockableServiceData*> _expiredData;
        LockableServiceData* _newData; // temporary store until activated
        QString _current;
        size_t _max;
        size_t _maxChunkSize;
        size_t _space;
        unsigned long _id;

    friend class ServiceDataBufferTest;
};

} // namespace pelican

#endif // SERVICEDATABUFFER_H
