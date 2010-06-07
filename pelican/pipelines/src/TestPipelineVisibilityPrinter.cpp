#include "pelican/pipelines/TestPipelineVisibilityPrinter.h"
#include "pelican/modules/VisibilityPrinter.h"
#include "pelican/data/VisibilityData.h"

#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
TestPipelineVisibilityPrinter::TestPipelineVisibilityPrinter()
    : AbstractPipeline()
{
}


/**
 * @details
 */
TestPipelineVisibilityPrinter::~TestPipelineVisibilityPrinter()
{
}


/**
 * @details
 */
void TestPipelineVisibilityPrinter::init()
{
    _visPrinter = (VisibilityPrinter*) createModule("VisibilityPrinter");

    requestRemoteData("VisibilityData");
}


/**
 * @details
 */
void TestPipelineVisibilityPrinter::run(QHash<QString, DataBlob*>& remoteData)
{
    VisibilityData* vis = (VisibilityData*) remoteData["VisibilityData"];
    Q_ASSERT(vis != NULL);
    _visPrinter->run(vis);

    // Stop the pipeline driver.
    stop();
}

} // namespace pelican
