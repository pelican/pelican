#ifndef BASICFLAGGER_H
#define BASICFLAGGER_H

#include "pelican/modules/AbstractModule.h"
#include "pelican/data/VisibilityData.h"
#include "pelican/data/FlagTable.h"
#include "pelican/utility/ConfigNode.h"
#include <vector>

/**
 * @file BasicFlagger.h
 */

namespace pelican {

/**
 * @class BasicFlagger
 *
 * @brief
 * Provides basic flagging routines for visibility data.
 *
 * @details
 * The BasicFlagger module
 */

class BasicFlagger : public AbstractModule
{
        friend class BasicFlaggerTest;

    private:
        /// Minimum visibility level at which to flag, as a fraction of the median.
        real_t _minLevelMedian;

        /// Maximum visibility level at which to flag, as a fraction of the median.
        real_t _maxLevelMedian;

    public:
        /// Module constructor.
        BasicFlagger(const ConfigNode& config);

        /// Module destructor.
        ~BasicFlagger();

        /// Runs the module.
        void run(VisibilityData* visibilityData, FlagTable* flagTable);

    private:
        /// Flags the autocorrelations.
        void _flagAutocorrelations(
                const VisibilityData* visData,
                const complex_t* medians,
                const real_t minFraction,
                const real_t maxFraction,
                FlagTable* flagTable
        );

        /// Gets the autocorrelations.
        void _getAutocorrelations(const VisibilityData* visData,
                complex_t* aptr);

        /// Determines the median autocorrelation values.
        void _getMedians(
                const unsigned nAntennas,
                const unsigned nChannels,
                const unsigned nPols,
                const complex_t* aptr,
                complex_t* mptr
        );

        /// Moves bad antennas to the end of the visibility matrix.
        void _moveBadAntennas(VisibilityData* visData, FlagTable* flagTable);
};

bool complexCompareAbs(complex_t i, complex_t j);

} // namespace pelican

#endif // BASICFLAGGER_H
