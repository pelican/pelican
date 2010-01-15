#ifndef BASICFLAGGER_H
#define BASICFLAGGER_H

#include "AbstractModule.h"

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
};

} // namespace pelican

#endif // BASICFLAGGER_H 
