#include "TestOutputStreamer.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details TestOutputStreamer 
 */
TestOutputStreamer::TestOutputStreamer( const QString& id)
    : AbstractOutputStream(_config), _id(id)
{
}

TestOutputStreamer::TestOutputStreamer( const ConfigNode& config )
    : AbstractOutputStream(config), _config(config)
{
}
/**
 *@details
 */
TestOutputStreamer::~TestOutputStreamer()
{
}

void TestOutputStreamer::send(const QString& streamName, const DataBlob* dataBlob)
{
    _received[streamName] = dataBlob;
}

} // namespace pelican
