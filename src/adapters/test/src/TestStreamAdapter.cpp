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
    TestDataBlob* data = static_cast<TestDataBlob*>(_data);
    data->resize(_chunkSize);
    in->read( data->data(), (quint64)_chunkSize);
}

} // namespace pelican
