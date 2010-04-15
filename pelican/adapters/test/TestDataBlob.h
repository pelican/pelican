#ifndef TESTDATABLOB_H
#define TESTDATABLOB_H

#include <vector>
#include "pelican/data/DataBlob.h"

/**
 * @file TestDataBlob.h
 */

namespace pelican {

/**
 * @class TestDataBlob
 *  
 * @brief
 * Data blob to support unit testing.
 *
 * @details
 * A minimalist data blob that has no structure, just an empty buffer.
 */
class TestDataBlob : public DataBlob
{
    private:
        std::vector<char> _buffer;

    public:
        /// Constructs the test data blob.
        TestDataBlob() : DataBlob() {}

        /// Returns a pointer to the first element of the buffer.
        char* data() {return &_buffer[0];}

        /// Resizes the data buffer.
        void resize(size_t size) {
            _buffer.resize(size + 1);
            _buffer[size + 1] = '\0'; // Safety if someone tries to print.
        }
};

} // namespace pelican

#endif // TESTDATABLOB_H 
