#include "TestPipelineServer.h"
#include "modules/VisibilityPrinter.h"
#include "data/VisibilityData.h"

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
TestPipelineServer::TestPipelineServer()
    : AbstractPipeline()
{
    _counter = 0;
}


/**
 * @details
 */
TestPipelineServer::~TestPipelineServer()
{
}


/**
 * @details
 */
void TestPipelineServer::init()
{
    setName("TestPipelineServer");
    _visPrinter = (VisibilityPrinter*) createModule("VisibilityPrinter");

    requestRemoteData("VisibilityData");
}


/**
 * @details
 */
void TestPipelineServer::run(QHash<QString, DataBlob*>& remoteData)
{
    VisibilityData* vis = (VisibilityData*) remoteData["VisibilityData"];
    Q_ASSERT(vis != NULL);
    _visPrinter->run(vis);
    sleep(1);

    // Stop the pipeline driver if we have gone through 10 iterations.
    ++_counter;
    if (_counter == 10)
        stop();
}

} // namespace pelican
