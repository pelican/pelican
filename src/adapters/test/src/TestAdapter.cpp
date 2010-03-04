#include "TestAdapter.h"
#include "TestDataBlob.h"


#include "utility/memCheck.h"

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
    in->read( static_cast<TestDataBlob*>(_data)->data(), (quint64)_chunkSize);
}

} // namespace pelican
