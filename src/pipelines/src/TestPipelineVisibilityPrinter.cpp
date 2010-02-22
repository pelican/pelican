#include "TestPipelineVisibilityPrinter.h"
#include "modules/VisibilityPrinter.h"

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
    _visPrinter = static_cast<VisibilityPrinter*>(createModule("VisibilityPrinter"));
}



/**
 * @details
 */
void TestPipelineVisibilityPrinter::run(QHash<QString, DataBlob*>& data)
{
    _visPrinter->run(data);

    /* Stop the pipeline driver */
    stop();
}



} // namespace pelican
