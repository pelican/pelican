#ifndef TESTDATABLOB_H
#define TESTDATABLOB_H

#include <QVector>
#include "DataBlob.h"

/**
 * @file TestDataBlob.h
 */

namespace pelican {

/**
 * @class TestDataBlob
 *  
 * @brief
 * 
 * @details
 * 
 */

class TestDataBlob : public DataBlob
{
    public:
        /// Constructs a new test data blob.
        TestDataBlob();

        /// Destroys the test data blob.
        ~TestDataBlob();

    public:
        QVector<float> data;

    private:
};

} // namespace pelican
#endif // TESTDATABLOB_H 
