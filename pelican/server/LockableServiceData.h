#ifndef LOCKABLESERVICEDATA_H
#define LOCKABLESERVICEDATA_H

#include <cstring>
#include "pelican/server/AbstractLockableData.h"

/**
 * @file LockableData.h
 */

namespace pelican {

class Data;

/**
 * @class LockableData
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
        /// Constructs a new LockableData object.
        LockableServiceData(const QString& name = "", void* data = 0,
                size_t size = 0, QObject* parent = 0);

        /// Destroys the LockableData object.
        virtual ~LockableServiceData() {}

        /// Return the size of the stored data.
        size_t size() const {return _data->size();}

        /// Sets the size of the stored data.
        //void setSize(size_t);

        /// Returns a pointer to the beginning of the memory block.
        void* memoryLocation() {return _data->operator*();}

        /// Return the name of the data.
        QString name() const {return _data->name();}

        /// Returns true if there isany valid data in the object.
        virtual bool isValid() const {return _data->isValid();}

    private:
        /// Disallow the copy constructor.
        LockableServiceData(const LockableServiceData&);
};

} // namespace pelican

#endif // LOCKABLESERVICEDATA_H
