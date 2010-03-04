#ifndef TESTDATABLOB_H
#define TESTDATABLOB_H

#include <vector>
#include "data/DataBlob.h"

/**
 * @file TestDataBlob.h
 */

namespace pelican {

/**
 * @class TestDataBlob
 *  
 * @brief
 *    Datablob to support unit testing
 * @details
 *    A minimalist datablob that has no structure - just an empty buffer
 * 
 */

class TestDataBlob : public DataBlob
{
    public:
        TestDataBlob(  );
        ~TestDataBlob();
        char* data();
        void resize(size_t size);

    private:
        std::vector<char> _buf;
};

} // namespace pelican
#endif // TESTDATABLOB_H 
