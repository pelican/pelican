#include "TestAdapter.h"
#include "TestDataBlob.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details TestAdapter 
 */
TestAdapter::TestAdapter(const pelican::ConfigNode& config)
    : AbstractServiceAdapter(config)
{
}

/**
 *@details
 */
TestAdapter::~TestAdapter()
{
}

void TestAdapter::deserialise(QIODevice* in)
{
    TestDataBlob* data = static_cast<TestDataBlob*>(_data);
    data->resize(_chunkSize);
    in->read( data->data(), (quint64)_chunkSize);
}

} // namespace pelican
