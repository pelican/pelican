/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef REALDATA_H
#define REALDATA_H

/**
 * @file ArrayData.h
 */

#include "data/DataBlob.h"
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
