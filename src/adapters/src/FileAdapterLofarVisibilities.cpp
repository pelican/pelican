#include "adapters/FileAdapterLofarVisibilities.h"
#include "utility/ConfigNode.h"

#include "utility/memCheck.h"

namespace pelican {


/**
 * @details
 */
FileAdapterLofarVisibilities::FileAdapterLofarVisibilities(const ConfigNode& config)
: AbstractAdapter(config)
{
    // Grab config for the adapter
    _nAnt = config.getOption("antenna", "number", "96").toUInt();
    _nChan = config.getOption("channels", "number", "512").toUInt();
    _nPol = config.getOption("polarisations", "number", "2").toUInt();
    _dataBytes = config.getOption("dataBytes", "number", "4").toUInt();
    _data = NULL;
}


/**
 * @details
 */
FileAdapterLofarVisibilities::~FileAdapterLofarVisibilities()
{
}


/**
 * @details
 * Method for reading a Lofar station visibility file from a QDataStream into
 * a visibility data object.
 *
 * !......ONLY AN EXAMPLE AT THE MOMENT....!
 */
void FileAdapterLofarVisibilities::deseralise(QDataStream& in) const
{
    complex_t* data = _data->ptr();

    // Assuming checkboard ant(XX/YY) per channel ?!
    for (unsigned c = 0; c < _nChan; c++) {
        for (unsigned j = 0; j < _nAnt; j++) {
            for (unsigned i = 0; i < _nAnt; i++) {
                for (unsigned p = 0; p < _nPol; c++) {
                    in >> data[i].real();
                    in >> data[i].imag();
                }
            }
        }
    }
}

} // namespace pelican
