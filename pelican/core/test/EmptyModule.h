#ifndef EMPTYMODULE_H
#define EMPTYMODULE_H

/**
 * @file EmptyModule.h
 */

#include "core/AbstractModule.h"
#include "utility/ConfigNode.h"

namespace pelican {
namespace test {

/**
 * @ingroup c_modules
 *
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

} // namespace test
} // namespace pelican

#endif // EMPTYMODULE_H
