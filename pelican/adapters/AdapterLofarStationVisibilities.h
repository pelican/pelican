#ifndef ADAPTERLOFARSTATIONVISIBILITIES_H
#define ADAPTERLOFARSTATIONVISIBILITIES_H

#include "pelican/adapters/AbstractStreamAdapter.h"
#include "pelican/utility/constants.h"
#include <QDataStream>

/**
 * @file AdapterLofarStationVisibilities.h
 */

namespace pelican {

class ConfigNode;
class VisibilityData;

/**
 * @class AdapterLofarStationVisibilities
 *
 * @brief
 * Adapter to deserialise a set of LOFAR station visibilities into a
 * PELICAN visibility data blob.
 *
 * @details
 * Stream adapter to deserialise a visibility amplitude data set parameterised
 * by number of antennas, number of channels, and number of polarisations into
 * a PELICAN VisibilityData blob.
 * The initial size of the visibility data blob is set from the configuration
 * options by may be updated by passing appropriate server data blobs (not yet
 * implemented) to the adapter configuration.
 */

class AdapterLofarStationVisibilities : public AbstractStreamAdapter
{
    private:
        friend class AdapterLofarStationVisibilitiesTest;

    public:
        /// Constructs the adapter
        AdapterLofarStationVisibilities(const ConfigNode& config);

        /// Destroys the adapter
        ~AdapterLofarStationVisibilities();

    protected:
        /// Method to deserialise a LOFAR visibility file data stream.
        void deserialise(QIODevice* in);

    private:
        /// Updates and checks the size of the visibility data being read into.
        void _setData();

        /// Updates dimensions of visibility data being deserialised based
        /// on information contained in the service passed in the adapter config.
        void _updateDimensions();

    private:
        double _timeStart;      ///< Start time (Julian Day, UT1).
        double _timeDelta;      ///< Time delta (seconds).
        unsigned _nAnt;         ///< Number of antennas in the chunk.
        unsigned _nChan;        ///< Number of channels in the chunk.
        unsigned _nPol;         ///< Number of polarisations in the chunk.
        unsigned _dataBytes;    ///< Number of bytes per data point.
        Polarisation _polarisation; ///< The polarisation.
        std::vector<unsigned> _channels;     ///< List of channels in the data.
        bool _rowMajor;         ///< Set if the data is C-ordered (row major).
        VisibilityData* _vis;   ///< Pointer to visibility data blob being read into.
};

} // namespace pelican

#endif // FILEADAPTERLOFARVISIBILITIES_H
