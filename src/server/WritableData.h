#ifndef WRITABLEDATA_H
#define WRITABLEDATA_H


/**
 * @file WritableData.h
 */

namespace pelican {

class AbstractLockableData;

/**
 * @class WritableData
 *  
 * @brief
 *    Provides locked access to an area of memory
 *    to enable writing
 * @details
 *    The data will become unlocked on destruction
 *    and emit a signal when there are no locks upon it
 *
 */

class WritableData 
{
    public:
        WritableData(AbstractLockableData* d);
        ~WritableData();
        void write(const void* dataBuffer, size_t size, size_t offset = 0);
        AbstractLockableData* data();
        /// returns true if there is a valid Data object
        bool isValid() const;

    private:
        AbstractLockableData* _data;
};

} // namespace pelican
#endif // WRITABLEDATA_H 
