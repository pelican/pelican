#include "modules/AbstractModule.h"

namespace pelican {

/// Checks the polarisation consitency between the selection in the
/// XML configuration and polarisation of input data blobs
void AbstractModule::checkPolarisationConsitency(DataBlob::pol_t dataPol,
        DataBlob::pol_t modulePol, QString moduleName)
{
    if (dataPol != DataBlob::POL_BOTH) {
        if (modulePol == DataBlob::POL_X && dataPol != VisibilityData::POL_X)
            throw QString("%1: Polarisation selection X is inconsistent with input data").arg(moduleName);
        if (modulePol == VisibilityData::POL_Y && dataPol != VisibilityData::POL_Y)
            throw QString("%1: Polarisation selection Y is inconsistent with input data").arg(moduleName);
    }
}

} // namespace pelican
