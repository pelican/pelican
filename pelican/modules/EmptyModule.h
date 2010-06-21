#ifndef EMPTYMODULE_H
#define EMPTYMODULE_H

#include "pelican/modules/AbstractModule.h"
#include "pelican/utility/ConfigNode.h"

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
        EmptyModule(const ConfigNode& config) : AbstractModule(config) {}

        /// Module destructor.
        ~EmptyModule() {}

};

PELICAN_DECLARE_MODULE(EmptyModule)

} // namespace pelican

#endif // EMPTYMODULE_H
