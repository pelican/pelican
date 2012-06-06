#ifndef ABSTRACTMODULE_H
#define ABSTRACTMODULE_H

/**
 * @file AbstractModule.h
 */

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/FactoryRegistrar.h"
#include "pelican/data/DataBlob.h"
#include <QtCore/QString>
#include <algorithm>
#include <iostream>
#include <vector>
using std::vector;

namespace pelican {
class AbstractPipeline;

/// This macro is used to register the named module type.
#define PELICAN_DECLARE_MODULE(type) PELICAN_DECLARE(AbstractModule, type)

/**
 * @ingroup c_core
 *
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

        // reference to the pipeline the module is running in
        AbstractPipeline* _pipeline;

    public:
        /// Creates a new abstract Pelican module with the given configuration.
        PELICAN_CONSTRUCT_TYPES(ConfigNode)
        AbstractModule(const ConfigNode& config);

        /// Destroys the module (virtual).
        virtual ~AbstractModule();

        /// set the pipeline context
        void setPipeline( AbstractPipeline* p ) { _pipeline = p; };

        /// create a single DataBlob of the specified type
        DataBlob* createBlob(const QString& type) const;

        /** 
         * @details
         * Sends data to the output streams managed by the OutputStreamManger
         *
         * @param[in] DataBlob to be sent.
         * @param[in] name of the output stream (defaults to DataBlob->type()).
         */
        void dataOutput( const DataBlob*, const QString& stream = "" ) const;

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
