#include "modules/ImagerDFT.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Module constructor.
 */
ImagerDFT::ImagerDFT(const QDomElement& config)
    : AbstractModule(config)
{
}

/**
 * @details
 * Module destructor.
 */
ImagerDFT::~ImagerDFT()
{
}


/**
 * @details
 * Perform a discrete Fourier transform to form an image from the visibily data.
 */
void _imageDFT()
{
}





} // namespace pelican
