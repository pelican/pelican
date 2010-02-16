#include "MyPipeline.h"
#include "modules/VisibilityPrinter.h"

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
MyPipeline::MyPipeline()
    : AbstractPipeline()
{
}


/**
 * @details
 */
MyPipeline::~MyPipeline()
{
}


/**
 * @details
 */
void MyPipeline::init()
{
    setName("MyPipeline");
    _visPrinter = static_cast<VisibilityPrinter*>(createModule("VisibilityPrinter"));
}



/**
 * @details
 */
void MyPipeline::run(QHash<QString, DataBlob*>& data)
{
    _visPrinter->run(data);
}



} // namespace pelican
