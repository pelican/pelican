#include "pelican/modules/AbstractModule.h"

#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Checks the polarisation consistency between the selection in the
 * XML configuration and polarisation of input data blobs.
 */
void AbstractModule::checkPolarisationConsitency(const Polarisation dataPol,
        const Polarisation modulePol) const
{
    QString moduleName = _config.getDomElement().tagName();

    if (dataPol != POL_BOTH) {
        if (modulePol == POL_X && dataPol != POL_X)
            throw QString("%1: Polarisation selection X is inconsistent with input data").arg(moduleName);
        if (modulePol == POL_Y && dataPol != POL_Y)
            throw QString("%1: Polarisation selection Y is inconsistent with input data").arg(moduleName);
    }
}

} // namespace pelican
