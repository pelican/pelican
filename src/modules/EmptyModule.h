#ifndef EMPTYMODULE_H
#define EMPTYMODULE_H

#include "core/AbstractModule.h"

class QDomElement;

/**
 * @file EmptyModule.h
 */

namespace pelican {

/**
 * @class EmptyModule
 *  
 * @brief
 * Module which does nothing and requires no data.
 * 
 * @details
 * This is part of the module unit tests.
 */
class EmptyModule : public AbstractModule
{
    public:
        /// Module constructor.
        EmptyModule(const QDomElement& config);

        /// Module destructor.
        ~EmptyModule();

        /// Runs the module.
        void run(QHash<QString, DataBlob*>& data) {}
};

} // namespace pelican

#endif // EMPTYMODULE_H
