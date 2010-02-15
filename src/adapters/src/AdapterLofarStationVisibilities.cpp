#include "adapters/AdapterLofarStationVisibilities.h"
#include "utility/ConfigNode.h"

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
void AdapterLofarStationVisibilities::deseralise(QDataStream& in)
{
    _setVisibilityData();

    complex_t* vis = _vis->ptr();

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


/**
 * @detail
 * Sets the size of the visibility data chunk which can be updated from
 * service data.
 * Checks the dimensions of the visibility data to be read matches the stream
 * chunk size.
 * Resizes the visibility data blob as required.
 */
void AdapterLofarStationVisibilities::_setVisibilityData()
{
    if (!_serviceData.empty()) {
        // get new sizes for nAnt, nChan, nPol, dataBytes from service data...
    }

    // Check the expected size due to data dimensions matches the stream chunk size
    unsigned dataSize = _nAnt * _nAnt * _nChan * _nPol * _dataBytes;
    if (dataSize != _chunkSize) {
        throw QString("Stream chunk size does not match data current dimensions");
    }

    // Resize the visibility data being read into to match the adapter dimensions
    _vis = static_cast<VisibilityData*>(_data);
    _vis->resize(_nAnt, _nChan, _nPol);
}


} // namespace pelican
