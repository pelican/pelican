#ifndef BASICFLAGGER_H
#define BASICFLAGGER_H

#include "AbstractModule.h"
#include "data/VisibilityData.h"
#include <vector>

class QDomElement;

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
 * 
 */
class BasicFlagger : public AbstractModule
{
        friend class BasicFlaggerTest;

    private:
        /// Flag set if using a separate flag table.
        bool _useFlagTable;

        /// Minimum visibility level at which to flag, as a fraction of the median.
        real_t _minLevelMedian;

        /// Maximum visibility level at which to flag, as a fraction of the median.
        real_t _maxLevelMedian;

    public:
        /// Module constructor.
        BasicFlagger(const QDomElement& config);

        /// Module destructor.
        ~BasicFlagger();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data);

    private:
        /// Gets the autocorrelations.
        void _getAutocorrelations(const VisibilityData* visData,
                std::vector<complex_t>& autocorr);

        /// Determines the median autocorrelation values.
        void _getMedian(const unsigned nAntennas,
                const unsigned nChannels, const unsigned nPols,
                std::vector<complex_t>& autocorr,
                std::vector<complex_t>& medians);
};

bool complexCompareAbs(complex_t i, complex_t j);

} // namespace pelican

#endif // BASICFLAGGER_H 
