#ifndef TESTSERVICEADAPTER_H
#define TESTSERVICEADAPTER_H

/**
 * @file TestServiceAdapter.h
 */

#include "pelican/core/AbstractServiceAdapter.h"

namespace pelican {
namespace test {

/**
 * @ingroup t_core
 *
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

} // namespace test
} // namespace pelican
#endif // TESTSERVICEADAPTER_H
