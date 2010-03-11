#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include <QHash>
#include <QString>
#include "utility/ConfigNode.h"
#include "data/DataRequirements.h"
#include "data/DataBlob.h"

/**
 * @file AbstractModule.h
 */

namespace pelican {

/**
 * @class AbstractModule
 *
 * @brief
 * Abstract base class for all pipeline modules.
 *
 * @details
 * This is the base class for all Pelican pipeline modules, which provide
 * functionality to Pelican pipelines. Inherit this class to create a new
 * module type.
 */
class AbstractModule
{
    private:
        /// The configuration node for the module.
        const ConfigNode* _config;

    public:
        /// Creates a new abstract Pelican module with the given configuration.
        AbstractModule(const ConfigNode& config) {
            _config = &config;
        }

        /// Destroys the module (virtual).
        virtual ~AbstractModule() {}
};

} // namespace pelican

#endif // ABSTRACTMODULE_H
