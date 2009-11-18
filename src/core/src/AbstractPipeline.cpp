#include "AbstractPipeline.h"

namespace pelican {

/**
 * @details
 * AbstractPipeline constructor.
 */
AbstractPipeline::AbstractPipeline()
{
}

/**
 * @details
 * AbstractPipeline destructor.
 */
AbstractPipeline::~AbstractPipeline()
{
}

/**
 * @details
 * Return the data requirements for the pipeline.
 */
const DataRequirements& AbstractPipeline::dataRequired() const
{
    return _data;
}

} // namespace pelican

