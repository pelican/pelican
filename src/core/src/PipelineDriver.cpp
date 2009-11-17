#include "PipelineDriver.h"

namespace pelican {

/**
 * @details
 * PipelineDriver constructor.
 */
PipelineDriver::PipelineDriver(int argc, char** argv)
{
}

/**
 * @details
 * PipelineDriver destructor.
 */
PipelineDriver::~PipelineDriver()
{
}

/**
 * @details
 * Registers the pipeline with the driver.
 */
void PipelineDriver::registerPipeline(AbstractPipeline *pipeline)
{
}

/**
 * @details
 * Returns a reference to the module factory.
 */
ModuleFactory& PipelineDriver::moduleFactory() const
{
}

/**
 * @details
 * Starts the data flow through the pipelines.
 */
void PipelineDriver::start()
{
}

} // namespace pelican

