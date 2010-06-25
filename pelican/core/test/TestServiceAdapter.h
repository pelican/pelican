#ifndef TESTSERVICEADAPTER_H
#define TESTSERVICEADAPTER_H

#include "pelican/core/AbstractServiceAdapter.h"

/**
 * @file TestServiceAdapter.h
 */

namespace pelican {

/**
 * @class TestServiceAdapter
 *  
 * @brief
 * Simple pass through adapter for use with the TestBataBlob object.
 *
 * @details
 * 
 */
class TestServiceAdapter : public AbstractServiceAdapter
{
    public:
        /// Constructs the test service adapter.
        TestServiceAdapter( const ConfigNode& config = ConfigNode() )
        : AbstractServiceAdapter(config) {}

        /// Destroys the test service adapter.
        ~TestServiceAdapter() {}

        /// Deserialises the data into a TestDataBlob.
        virtual void deserialise(QIODevice* in);
};

} // namespace pelican

#endif // TESTSERVICEADAPTER_H
