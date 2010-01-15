#ifndef BASICFLAGGER_H
#define BASICFLAGGER_H

#include "core/AbstractModule.h"

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
    public:
        /// Module constructor.
        BasicFlagger(const QDomElement& config);

        /// Module destructor.
        ~BasicFlagger();

        /// Runs the module.
        void run() {}
};

} // namespace pelican

#endif // BASICFLAGGER_H 
