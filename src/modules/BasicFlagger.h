#ifndef BASICFLAGGER_H
#define BASICFLAGGER_H

#include "AbstractModule.h"
#include "data/DataBlob.h"

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
};

} // namespace pelican

#endif // BASICFLAGGER_H 
