#include "modules/AbstractModule.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Checks the polarisation consistency between the selection in the
 * XML configuration and polarisation of input data blobs.
 */
void AbstractModule::checkPolarisationConsitency(const DataBlob::pol_t dataPol,
        const DataBlob::pol_t modulePol) const
{
    QString moduleName = _config.getDomElement().tagName();

    if (dataPol != DataBlob::POL_BOTH) {
        if (modulePol == DataBlob::POL_X && dataPol != DataBlob::POL_X)
            throw QString("%1: Polarisation selection X is inconsistent with input data").arg(moduleName);
        if (modulePol == DataBlob::POL_Y && dataPol != DataBlob::POL_Y)
            throw QString("%1: Polarisation selection Y is inconsistent with input data").arg(moduleName);
    }
}

} // namespace pelican
