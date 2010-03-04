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
                complex_t* autoCorrelations, complex_t* ww, double* rWork,
                double* eignenvalues, const unsigned nEigenvaluesUsed,
                complex_t *work, int lWork, const unsigned nIter);

        /// Zero diagonals of matrix
        void _zeroDiagonals(complex_t* matrix, const unsigned size);

        /// Gets the diagonals of a square matrix
        void _setDiagonals(complex_t* matrix, const unsigned size,
                const complex_t* diagonals);

        /// Extract and check data blobs from the data hash
        void _fetchDataBlobs(QHash<QString, DataBlob*>& data);

        /// Grab configuration options from the config node
        void _getConfiguration(const ConfigNode& config);

        /// Computes the complex gains.
        void _computeComplexGains (
                const unsigned n_a,
                const unsigned ne,
                double* Dz,
                complex_t* model,
                complex_t* Vz,
                complex_t* work,
                int lWork,
                double* rWork,
                complex_t* gains
        );

        /// Builds the corrected visibility set using the complex gains.
        void _buildCorrectedVisibilities (
                const unsigned n_a,
                const complex_t* vis,
                const complex_t* gains,
                const complex_t* sigma_n,
                complex_t* visCorrected
        );

    private:
        VisibilityData* _vis;
        ModelVisibilityData* _modelVis;
        AntennaPositions* _antPos;

        unsigned _nEigenvalues;
        double _tolerance;
        unsigned _iterations;
        std::vector<Source> _sources;
        std::vector<unsigned> _channels;
        int _freqRefChannel;                ///< Frequency reference channel
        double _freqRef;                    ///< Reference frequency
        double _freqDelta;                  ///< Frequency delta
        pol_t _polarisation;
};

} // namespace pelican

#endif // ZENITHCALIBRATER_H
