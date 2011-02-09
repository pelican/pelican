#ifndef TESTSTREAMADAPTER_H
#define TESTSTREAMADAPTER_H

/**
 * @file TestStreamAdapter.h
 */

#include "pelican/core/AbstractStreamAdapter.h"

namespace pelican {
namespace test {

/**
 * @ingroup t_core
 *
 * @class TestStreamAdapter
 *
 * @brief
 * Simple pass through adapter for use with the TestBataBlob object.
 *
 * @details
 */

class TestStreamAdapter : public AbstractStreamAdapter
{
    public:
        /// Constructs the test stream adapter.
        TestStreamAdapter( const ConfigNode& config = ConfigNode() )
        : AbstractStreamAdapter(config) {}

        /// Destroys the test stream adapter.
        ~TestStreamAdapter() {}

        /// Deserialises the data into a TestDataBlob.
        virtual void deserialise(QIODevice* in);
};

} // namespace test
} // namespace pelican
#endif // TESTSTREAMADAPTER_H
