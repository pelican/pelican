#include "pelican/pipelines/TestPipelineServer.h"

#include "pelican/modules/VisibilityPrinter.h"
#include "pelican/data/VisibilityData.h"

#include "pelican/utility/memCheck.h"

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
//    sleep(1);

    // Stop the pipeline driver if we have gone through 10 iterations.
    ++_counter;
    if (_counter == 3000)
        stop();
}

} // namespace pelican
