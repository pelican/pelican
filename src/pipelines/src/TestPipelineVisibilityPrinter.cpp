#include "TestPipelineVisibilityPrinter.h"
#include "modules/VisibilityPrinter.h"
#include "data/VisibilityData.h"

#include "utility/memCheck.h"

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
    setName("TestPipelineVisibilityPrinter");
    _visPrinter = (VisibilityPrinter*) createModule("VisibilityPrinter");

    requestRemoteData("VisibilityData");
}


/**
 * @details
 */
void TestPipelineVisibilityPrinter::run(QHash<QString, DataBlob*>& remoteData)
{
    VisibilityData* vis = (VisibilityData*) remoteData["VisibilityData"];
    _visPrinter->run(vis);

    // Stop the pipeline driver.
    stop();
}

} // namespace pelican
