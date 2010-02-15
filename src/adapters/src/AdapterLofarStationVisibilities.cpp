#include "adapters/AdapterLofarStationVisibilities.h"
#include "utility/ConfigNode.h"
#include "data/VisibilityData.h"

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
AdapterLofarStationVisibilities::AdapterLofarStationVisibilities(const ConfigNode& config)
: AbstractStreamAdapter(config)
{
    // Grab configuration for the adapter setting sensible defaults
    _nAnt = config.getOption("antenna", "number", "96").toUInt();
    _nChan = config.getOption("channels", "number", "512").toUInt();
    _nPol = config.getOption("polarisations", "number", "2").toUInt();
    _dataBytes = config.getOption("dataBytes", "number", "4").toUInt();
}


/**
 * @details
 */
AdapterLofarStationVisibilities::~AdapterLofarStationVisibilities()
{
}


/**
 * @details
 * Method for reading a Lofar station visibility file from a QDataStream into
 * a visibility data object.
 *
 * !......ONLY AN EXAMPLE AT THE MOMENT....!
 */
void AdapterLofarStationVisibilities::deseralise(QDataStream& in) const
{
    VisibilityData *data = static_cast<VisibilityData*>(_data);
    complex_t* vis = data->ptr();

    // Assuming checkboard ant(XX/YY) per channel ?!
    for (unsigned c = 0; c < _nChan; c++) {
        for (unsigned j = 0; j < _nAnt; j++) {
            for (unsigned i = 0; i < _nAnt; i++) {
                for (unsigned p = 0; p < _nPol; c++) {
                    in >> vis[i].real();
                    in >> vis[i].imag();
                }
            }
        }
    }
}

} // namespace pelican
