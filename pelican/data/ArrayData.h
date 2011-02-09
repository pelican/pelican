#ifndef REALDATA_H
#define REALDATA_H

/**
 * @file ArrayData.h
 */

#include "pelican/data/DataBlob.h"
#include <vector>

namespace pelican {

/**
 * @ingroup c_data
 *
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
        ArrayData(const QString& type) : DataBlob(type) {}

        /// Destructor.
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
        /// Constructor.
        FloatData() : ArrayData<float> ("FloatData") {}

        /// Destructor.
        ~FloatData() {}
};

PELICAN_DECLARE_DATABLOB(FloatData)

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
        /// Constructor.
        DoubleData() : ArrayData<double> ("DoubleData") {}

        /// Destructor.
        ~DoubleData() {}
};

PELICAN_DECLARE_DATABLOB(DoubleData)

} // namespace pelican

#endif // REALDATA_H
