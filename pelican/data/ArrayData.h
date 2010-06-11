#ifndef REALDATA_H
#define REALDATA_H

/**
 * @file ArrayData.h
 */

#include "pelican/data/DataBlob.h"
#include <vector>

namespace pelican {

/**
 * @class ArrayData
 *
 * @brief
 * Data blob to hold an array.
 *
 * @details
 * This data blob holds an array.
 */
template <class T>
class ArrayData : public DataBlob
{
    private:
        std::vector<T> _data;

    public:
        /// Constructor.
        ArrayData() : DataBlob() {}
        
        // Destructor.
        virtual ~ArrayData() {}

        /// Returns a pointer to the start of the data.
        T* ptr() { return (_data.size() > 0 ? &_data[0] : NULL); }
        
        /// Returns a pointer to the start of the data. (const. overload)
        const T* ptr() const { return (_data.size() > 0 ? &_data[0] : NULL); }

        /// Resizes the data blob.
        void resize(unsigned length) { _data.resize(length); }

        /// Returns the size of the data.
        unsigned size() { return _data.size(); }
};



/**
 * @class FloatData
 *
 * @brief
 * Data blob to hold an array of single precision floating point values.
 *
 * @details
 * Holds an array of single precision floating point values.
 */
class FloatData : public ArrayData<float>
{
    public:
        // Contructor.
        FloatData() : ArrayData<float> () {}
        
        // Destructor.
        ~FloatData() {}
};


/**
 * @class DoubleData
 *
 * @brief
 * Data blob to hold an array of dobule precision floating point values.
 *
 * @details
 * Holds an array of double precision floating point values.
 */
class DoubleData : public ArrayData<double>
{
    public:
        // Contructor.
        DoubleData() : ArrayData<double> () {}
        
        // Destructor.
        ~DoubleData() {}
};


} // namespace pelican

#endif // REALDATA_H
