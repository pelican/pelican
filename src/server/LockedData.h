#ifndef LOCKEDDATA_H
#define LOCKEDDATA_H


/**
 * @file LockedData.h
 */

namespace pelican {

class Data;

/**
 * @class LockedData
 *  
 * @brief
 *    A container that automatically locks and unlocks the data object
 *    to mark it as being used
 * @details
 * 
 */

class LockedData
{
    public:
        LockedData(Data* data );
        ~LockedData();
        bool isValid() const;

    private:
        Data* _data;
};

} // namespace pelican
#endif // LOCKEDDATA_H 
