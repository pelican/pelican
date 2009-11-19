#include "PipelineApplication.h"
#include "utility/memCheck.h"

#include "TestDataClient.h"

namespace pelican {


// class PipelineApplication 
PipelineApplication::PipelineApplication(int argc, char** argv)
{
}

PipelineApplication::~PipelineApplication()
{
}

void PipelineApplication::_init(const CoreOptions& opts)
{
    if(opts.dataClient().toLower() == "test" ) {
        _dataClient = new TestDataClient;
    } else {
        throw QString("Unknown data client type.");
    }
}

} // namespace pelican
