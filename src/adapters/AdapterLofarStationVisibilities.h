#ifndef ADAPTERLOFARSTATIONVISIBILITIES_H
#define ADAPTERLOFARSTATIONVISIBILITIES_H

#include "adapters/AbstractStreamAdapter.h"
#include <QDataStream>

/**
 * @file AdapterLofarStationVisibilities.h
 */

namespace pelican {

class ConfigNode;

/**
 * @class AdapterLofarStationVisibilities
 *  
 * @brief
 * 
 * @details
 */

class AdapterLofarStationVisibilities : public AbstractStreamAdapter
{
    public:
        /// Constructs the adapter
        AdapterLofarStationVisibilities(const ConfigNode& config);

        /// Destorys the adapter
        ~AdapterLofarStationVisibilities();

    protected:
        /// Method to deseralise a LOFAR visibility file data stream.
        virtual void deseralise(QDataStream& in) const;

    private:
        unsigned _nAnt;         ///< Number of antennas in the chunk
        unsigned _nChan;        ///< Number of channels in the chunk
        unsigned _nPol;         ///< Number of polarisations in the chunk
        unsigned _dataBytes;    ///< number of bytes per data point
};

} // namespace pelican

#endif // FILEADAPTERLOFARVISIBILITIES_H 
