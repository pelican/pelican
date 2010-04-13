#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include "utility/Config.h"
#include "utility/ConfigNode.h"

/**
 * @file AbstractFactory.h
 */

namespace pelican {

/**
 * @class AbstractFactory
 *
 * @brief
 *    An Abstract Factory Class for use is Pelican Factories
 * @details
 *    Intended for constructing objects that take a Config
 *    class in their constructor
 *
 */
class AbstractFactory
{
    public:
        AbstractFactory(const Config* config, const Config::TreeAddress& base);
        virtual ~AbstractFactory();

        /// Returns the tree node address that marks the start
        /// of the module configuration block.
        Config::TreeAddress configRoot() const { return _configRoot; }

        /// return the configuration node for a type (named type if supplied)
        ConfigNode configuration(const QString& type, const QString& name="") const;

    protected:
        const Config* _config;
        Config::TreeAddress _configRoot;
};

} // namespace pelican

#endif // ABSTRACTFACTORY_H
