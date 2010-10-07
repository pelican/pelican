#include "TestOutputStreamer.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 *@details TestOutputStreamer 
 */
TestOutputStreamer::TestOutputStreamer( const QString& id, 
                                        const ConfigNode& config)
    : AbstractOutputStream(config), _id(id), _config(config)
{
}

TestOutputStreamer::TestOutputStreamer( const ConfigNode& config )
    : AbstractOutputStream(config), _config(config)
{
    // Check configuration node is correct.
    if (config.type() != "TestOutputStreamer")
        throw QString("TestOutputStreamer: Wrong configuration node. Got \"%1\"").arg(config.type());
}
/**
 *@details
 */
TestOutputStreamer::~TestOutputStreamer()
{
}

void TestOutputStreamer::sendStream(const QString& streamName, const DataBlob* dataBlob)
{
    _received[streamName] = dataBlob;
}

} // namespace pelican
