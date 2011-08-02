#ifndef LOCKABLESERVICEDATA_H
#define LOCKABLESERVICEDATA_H

#include <cstring>
#include "pelican/server/AbstractLockableData.h"

/**
 * @file LockableServiceData.h
 */

namespace pelican {

class DataChunk;

/**
 * @ingroup c_server
 *
 * @class LockableServiceData
 *
 * @brief
 * Primary interface to access chunks of data in the server.
 *
 * @details
 * This class takes care of locking etc.
 */
class LockableServiceData : public AbstractLockableData
{
    Q_OBJECT

    public:
        /// Constructs a new LockableServiceData object.
        LockableServiceData(const QString& name = "", void* data = 0,
                size_t size = 0, QObject* parent = 0);

        /// Destroys the LockableServiceData object.
        virtual ~LockableServiceData() {}

        /// Return the size of the stored data.
        size_t size() const {return _data->size();}
       
        /// set the size of data (must be less than the maxSize)
        void setSize(size_t s ) { _data->setSize(s); }

        /// Returns a pointer to the beginning of the memory block.
        void* memoryLocation() {return _data->ptr();}

        /// Return the name of the data.
        QString name() const {return _data->name();}

        /// Returns true if there is any valid data in the object.
        virtual bool isValid() const {return _data->isValid();}

    private:
        /// Disallow the copy constructor.
        LockableServiceData(const LockableServiceData&);
};

} // namespace pelican

#endif // LOCKABLESERVICEDATA_H
