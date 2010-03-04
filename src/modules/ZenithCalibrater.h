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
class CorrectedVisibilityData;

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
        void _calibrate(
                const unsigned nAnt,
                const unsigned nEigenvaluesUsed,
                const unsigned nIterations,
                const double tolerance,
                int lWork,
                complex_t *work,
                double* rWork,
                complex_t* Vz,
                complex_t* Dz
        );

        /// Computes the complex gains.
        void _computeComplexGains(
                const unsigned nAnt,
                const unsigned ne,
                complex_t* Dz,
                complex_t* modelVis,
                complex_t* Vz,
                complex_t* work,
                int lWork,
                double* rWork,
                complex_t* gains
        );

        /// Builds the corrected visibility set using the complex gains.
        void _buildCorrectedVisibilities(
                const unsigned nAnt,
                const complex_t* rawVis,
                const complex_t* antennaGain,
                const complex_t* sigma_n,
                complex_t* correctedVis
        );

        /// Sets the diagonals of a square matrix
        void _setDiagonals(complex_t* matrix, const unsigned size,
                const complex_t* diagonals);

        /// Scales a vector.
        void _sqrtScaleVector(complex_t* vector, const unsigned size,
                const double value, complex_t* scaled);

        /// Extract and check data blobs from the data hash
        void _fetchDataBlobs(QHash<QString, DataBlob*>& data);

        /// Grab configuration options from the config node
        void _getConfiguration(const ConfigNode& config);

    private:
        VisibilityData* _rawVis;
        ModelVisibilityData* _modelVis;
        CorrectedVisibilityData* _correctedVis;

        unsigned _nEigenvaluesUsed;
        double _tolerance;
        unsigned _nIterations;

        std::vector<unsigned> _channels;
        pol_t _polarisation;

        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
};

} // namespace pelican

#endif // ZENITHCALIBRATER_H
