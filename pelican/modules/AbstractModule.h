#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/Factory.h"
#include "pelican/data/DataBlob.h"

#include <QString>
#include <algorithm>
#include <vector>

#include <iostream>

/**
 * @file AbstractModule.h
 */

namespace pelican {

/**
 * This macro is used to register the named module type with the factory.
 * It should be used within the global scope of the module's source file.
 *
 * @note
 * The macro expands to declare a dummy variable of the object's generator
 * type, which (when constructed), adds the type name to the creator's known
 * object types.
 *
 * The macro is placed within the global scope so that it is initialised as
 * soon as the program starts executing. It is placed within an anonymous
 * namespace so that the dummy creator variable is not accessible from outside
 * the file that instantiated it.
 */
#define PELICAN_DECLARE_MODULE(type) namespace {Creator<type, AbstractModule> reg(#type);}

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
        AbstractModule(const ConfigNode config) : _config(config) {}

        /// Destroys the module (virtual).
        virtual ~AbstractModule() {}

    protected:
        /// Checks the polarisation consistency between the selection in the
        /// XML configuration and polarisation of input data blobs.
        void checkPolarisationConsitency(const DataBlob::pol_t dataPol,
                const DataBlob::pol_t modulePol) const;

        /// Returns the index of the first occurrence of value in the data.
        template <typename T>
        unsigned findIndex(const T value, const std::vector<T>& data) const
        {
            int index = -1;
            for (unsigned i = 0; i < data.size(); i++) {
                if (data[i] == value) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                QString moduleName = _config.getDomElement().tagName();
                throw QString("%1: Index for selected value not found.").arg(moduleName);
            }

            return static_cast<unsigned>(index);
        }
};

} // namespace pelican

#endif // ABSTRACTMODULE_H
