#include "TestStreamAdapter.h"
#include "TestDataBlob.h"


#include "utility/memCheck.h"

namespace pelican {


/**
 *@details TestStreamAdapter 
 */
TestStreamAdapter::TestStreamAdapter(const pelican::ConfigNode& config)
    : AbstractStreamAdapter(config)
{
}

/**
 *@details
 */
TestStreamAdapter::~TestStreamAdapter()
{
}

void TestStreamAdapter::deserialise(QIODevice* in)
{
    in->read( static_cast<TestDataBlob*>(_data)->data(), (quint64)_chunkSize);
}

} // namespace pelican
