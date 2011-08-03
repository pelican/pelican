#ifndef LOCKEDDATA_H
#define LOCKEDDATA_H

#include <QtCore/QString>
#include "AbstractLockable.h"

/**
 * @file LockedData.h
 */

namespace pelican {

/**
 * @ingroup c_server
 *
 * @class LockedData
 *
 * @brief
 * A container that automatically locks and unlocks any
 * AbstractLockable object added to it.
 *
 * @details
 * On destruction the AbstractLockable read lock count will be less by one.
 */
class LockedData
{
    public:
        /// Constructs a new LockedData object.
        LockedData(const QString& name, AbstractLockable* data = 0);

        /// Destroys the LockedData object.
        ~LockedData();

        /// Copy constructor also locks the data also.
        LockedData(const LockedData& data);

        /// Returns true if the object contains valid data.
        bool isValid() const {return _data ? _data->isValid() : false;}

        /// Add a data object.
        void setData(AbstractLockable*);

        /// Return the locked object.
        AbstractLockable* object() const {return _data;}

        /// Return the name of the data.
        QString name() const {return _name;}

    private:
        AbstractLockable* _data;
        QString _name;
};

} // namespace pelican

#endif // LOCKEDDATA_H
