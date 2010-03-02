#ifndef LOCKEDDATA_H
#define LOCKEDDATA_H

#include <QString>

/**
 * @file LockedData.h
 */

namespace pelican {

class LockableData;
class Data;

/**
 * @class LockedData
 *  
 * @brief
 *    A container that automatically locks and unlocks any data object
 *    added to it
 * @details
 *    On addition to this container, the Data will be locked. On destruction
 *    the Data will be unlocked.
 * 
 */

class LockedData
{
    public:
        LockedData(const QString& name, LockableData* data = 0);
        ~LockedData();
        /// copy constructor also locks the data also
        LockedData(const LockedData& data);

        /// returns true if the object contains valid data
        bool isValid() const;

        /// add a data object
        void setData(LockableData*);

        /// returns the size of the data
        size_t size() const;

        /// return the data object
        LockableData* data() const;

        /// return the name of the data
        QString name() const { return _name; };

    private:
        LockableData* _data;
        QString _name;
};

} // namespace pelican
#endif // LOCKEDDATA_H 
