#ifndef WRITABLEDATA_H
#define WRITABLEDATA_H

/**
 * @file WritableData.h
 */

#include "pelican/server/AbstractLockableData.h"

namespace pelican {

/**
 * @ingroup c_server
 *
 * @class WritableData
 *
 * @brief
 * Provides locked access to an area of memory to enable writing.
 *
 * @details
 * The data will become unlocked on destruction
 * and emit a signal when there are no locks upon it
 */
class WritableData
{
    public:
        WritableData() : _data(0) {}

        WritableData(AbstractLockableData* d);

        ~WritableData();

        void write(const void* dataBuffer, size_t size, size_t offset = 0);

        AbstractLockableData* data() const {return _data;}

        /// Returns a pointer to the start of the memory block.
        void* ptr() {return _data->data()->data();}

        /// returns true if there is a valid Data object
        bool isValid() const {return _data ? _data->isValid() : false;}

        WritableData& operator=(const WritableData& other);

    private:
        AbstractLockableData* _data;
};

} // namespace pelican

#endif // WRITABLEDATA_H
