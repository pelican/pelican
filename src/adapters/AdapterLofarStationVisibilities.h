#ifndef ADAPTERLOFARSTATIONVISIBILITIES_H
#define ADAPTERLOFARSTATIONVISIBILITIES_H

#include "adapters/AbstractStreamAdapter.h"
#include "data/VisibilityData.h"
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
        void deseralise(QDataStream& in);

    private:
        /// Update the size of the visibility data being read
        void _resizeVisibilityData();

    private:
        unsigned _nAnt;         ///< Number of antennas in the chunk
        unsigned _nChan;        ///< Number of channels in the chunk
        unsigned _nPol;         ///< Number of polarisations in the chunk
        unsigned _dataBytes;    ///< number of bytes per data point
        VisibilityData* _vis;   ///< Pointer to visibility data blob being read into.
};

} // namespace pelican

#endif // FILEADAPTERLOFARVISIBILITIES_H 
