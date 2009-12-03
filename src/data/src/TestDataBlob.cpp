#include "TestDataBlob.h"


namespace pelican {

/**
 * @details
 * Constructs a test data blob.
 */
TestDataBlob::TestDataBlob()
    : DataBlob()
{
    data.fill(1, 100);
}

/**
 * @details
 * Data blob destructor.
 */
TestDataBlob::~TestDataBlob()
{
}

} // namespace pelican
