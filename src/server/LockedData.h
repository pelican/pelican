#ifndef LOCKEDDATA_H
#define LOCKEDDATA_H

#include <QList>

/**
 * @file LockedData.h
 */

namespace pelican {

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
        LockedData(Data* data = 0 );
        ~LockedData();
        bool isValid() const;
        void addData(Data*);
        void addData(const LockedData&);
        size_t size() const;
        QList<Data*> data() { return _data; };

    private:
        QList<Data*> _data;
};

} // namespace pelican
#endif // LOCKEDDATA_H 
