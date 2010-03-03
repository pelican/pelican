#ifndef ZENITHCALIBRATER_H
#define ZENITHCALIBRATER_H

/**
 * @file ZenithCalibrater.h
 */

#include "modules/AbstractModule.h"
#include "data/Source.h"
#include <vector>


namespace pelican {

class ConfigNode;
class VisibilityData;
class ModelVisibilityData;
class AntennaPositions;


/**
 * @class ZenithCalibrater
 *
 * @brief
 * Provides basic calibration routines for whole-sky views.
 *
 * @details
 *
 */
class ZenithCalibrater : public AbstractModule
{
    public:
        /// Module constructor.
        ZenithCalibrater(const ConfigNode& config);

        /// Module destructor.
        ~ZenithCalibrater();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        enum pol_t { POL_X, POL_Y, POL_BOTH };

        /// Calibration loop.
        void _calibrate(complex_t* visTemp, const unsigned nAnt,
                complex_t* autoCorrelations, complex_t* crossCorrelations,
                const unsigned nEigenvalues);

        /// Zero diagonals of matrix
        void _zeroDiagonals(complex_t* matrix, const unsigned size);

        /// Gets the diagonals of a square matrix
        void _setDiagonals(complex_t* matrix, const unsigned size,
                const complex_t* diagonals);

        /// Extract and check data blobs from the data hash
        void _fetchDataBlobs(QHash<QString, DataBlob*>& data);

        /// Grab configuration options from the config node
        void _getConfiguration(const ConfigNode& config);

    private:
        VisibilityData* _vis;
        ModelVisibilityData* _modelVis;
        AntennaPositions* _antPos;

        unsigned _nEigenvalues;
        double _tolerance;
        std::vector<Source> _sources;
        std::vector<unsigned> _channels;
        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
        pol_t _polarisation;
};

} // namespace pelican

#endif // ZENITHCALIBRATER_H
