#ifndef LOCKABLEDATA_H
#define LOCKABLEDATA_H

#include <cstring>
#include "AbstractLockableData.h"

/**
 * @file LockableData.h
 */

namespace pelican {

class Data;

/**
 * @class LockableData
 *  
 * @brief
 *    Primary interface to access Chunks of data in the server
 * @details
 *    This class takes care of locking etc.
 */

class LockableData : public AbstractLockableData
{

    Q_OBJECT

    public:
        LockableData(const QString& name="", void* data=0, size_t size=0, QObject* parent=0);
        virtual ~LockableData();
        /// return the size of the stored data
        size_t size() const;
        /// sets the size of the stored data
        //void setSize(size_t);

        /// returns a pointer to the beginning of the memory block
        void* memoryLocation();

        /// return the name of the data
        QString name() const;

        /// returns true if there isany valid data in the object
        virtual bool isValid() const;

    private:
        LockableData(const LockableData&); // disallow the copy constructor

};

} // namespace pelican
#endif // LOCKABLEDATA_H 
