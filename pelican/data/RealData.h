#ifndef REALDATA_H
#define REALDATA_H

/**
 * @file RealData.h
 */

#include "pelican/data/DataBlob.h"
#include <vector>

namespace pelican {

/**
 * @class RealData
 *
 * @brief
 * Data blob to hold an array of real-valued data.
 *
 * @details
 * This data blob holds an array of real-valued data.
 */
class RealData : public DataBlob
{
    private:
        std::vector<real_t> _data;

    public:
        /// Constructor.
        RealData() : DataBlob() {}

        /// Returns a pointer to the start of the data.
        real_t* ptr() {return (_data.size() > 0 ? &_data[0] : NULL);}

        /// Resizes the data blob.
        void resize(int length) {_data.resize(length);}

        /// Returns the size of the data.
        unsigned size() {return _data.size();}
};

} // namespace pelican

#endif // REALDATA_H
