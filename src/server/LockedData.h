#ifndef LOCKEDDATA_H
#define LOCKEDDATA_H

#include <QString>

/**
 * @file LockedData.h
 */

namespace pelican {

class AbstractLockable;

/**
 * @class LockedData
 *  
 * @brief
 *    A container that automatically locks and unlocks any AbstractLockable object
 *    added to it
 * @details
 *    On destruction the AbstractLockable read lock count will be less by one.
 * 
 */

class LockedData
{
    public:
        LockedData(const QString& name, AbstractLockable* data = 0);
        ~LockedData();
        /// copy constructor also locks the data also
        LockedData(const LockedData& data);

        /// returns true if the object contains valid data
        bool isValid() const;

        /// add a data object
        void setData(AbstractLockable*);

        /// return the locked object
        AbstractLockable* object() const;

        /// return the name of the data
        QString name() const { return _name; };

    private:
        AbstractLockable* _data;
        QString _name;
};

} // namespace pelican
#endif // LOCKEDDATA_H 
