#ifndef DATABLOBEXAMPLE_H
#define DATABLOBEXAMPLE_H

/**
 * @file DataBlobExample.h
 */

#include "pelican/data/DataBlob.h"
#include <vector>

using namespace pelican;

/**
 * @ingroup c_examples
 *
 * @class DataBlobExample
 *
 * @brief
 * Data blob to hold an array of floating-point data.
 *
 * @details
 * This data blob holds an array of floating-point data.
 */

/*
 * This data blob holds an array of floating-point data.
 */
class DataBlobExample : public DataBlob
{
    public:
        // Constructs an example data blob.
        DataBlobExample() : DataBlob("DataBlobExample") {}

        /// Returns a const pointer to the start of the data.
        const float* data() const { return (_data.size() > 0 ? &_data[0] : 0); }

        /// Returns a pointer to the start of the data.
        float* data() { return (_data.size() > 0 ? &_data[0] : 0); }

        // Resizes the data store provided by the data blob.
        void resize(unsigned length) { _data.resize(length); }

        // Returns the size of the data.
        unsigned size() const { return _data.size(); }

    private:
        std::vector<float> _data; // The actual data array.
};

PELICAN_DECLARE_DATABLOB(DataBlobExample)

// Other example data blobs used by the example pipeline.
typedef DataBlobExample DataBlobExample1;
typedef DataBlobExample DataBlobExample2;

PELICAN_DECLARE_DATABLOB(DataBlobExample1)
PELICAN_DECLARE_DATABLOB(DataBlobExample2)

#endif // DATABLOBEXAMPLE_H
