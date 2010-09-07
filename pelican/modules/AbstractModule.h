#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/FactoryRegistrar.h"
#include "pelican/utility/constants.h"
#include "pelican/data/DataBlob.h"

#include <QtCore/QString>
#include <algorithm>
#include <vector>

#include <iostream>

using std::vector;

/**
 * @file AbstractModule.h
 */

namespace pelican {

/**
 * This macro is used to register the named module type.
 */
#define PELICAN_DECLARE_MODULE(type) PELICAN_DECLARE(AbstractModule, type)

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
        ConfigNode _config;

    public:
        /// Creates a new abstract Pelican module with the given configuration.
        PELICAN_CONSTRUCT_TYPES(ConfigNode)
        AbstractModule(const ConfigNode config) : _config(config) {}

        /// Destroys the module (virtual).
        virtual ~AbstractModule() {}

    protected:
        /// Returns the index of the first occurrence of value in the data.
        template <typename T>
        unsigned findIndex(T value, vector<T> const& data) const;

};




//------------------------------------------------------------------------------
// Method definitions.

template <typename T>
inline
unsigned AbstractModule::findIndex(T value, vector<T> const& data) const
{
    unsigned i;
    for (i = 0; i < data.size(); ++i) if (data[i] == value) break;

    if (i == data.size())
    {
        QString moduleName = _config.getDomElement().tagName();
        throw QString("%1: Index for selected value not found.").arg(moduleName);
    }
    return i;
}


} // namespace pelican

#endif // ABSTRACTMODULE_H
