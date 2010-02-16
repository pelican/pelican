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
 *
 * New modules must report their set of data requirements to the pipeline.
 * A number of protected methods exist for a module to define its set of data
 * requirements. In broad terms, there are three types of data:
 *
 * - Generated data is produced only by modules in a pipeline.
 * - Stream data is obtained from the client, updated on each pipeline iteration.
 * - Service data: obtained from the client, updated when necessary.
 *
 * In the module constructor, call addGeneratedData(), addStreamData() and
 * addServiceData() as needed, so the pipeline driver is aware of the data
 * blob types required. If the module generates its own data (for example, an
 * imaging module generating image data), use addGeneratedData() for this as
 * well.
 *
 * As an example, a flagging module might use in its constructor:
 *
 * \code
 * addStreamData("VisibilityData");
 * addGeneratedData("FlagTable");
 * \endcode
 *
 * The pipeline driver can then use the requiredDataAll(),
 * requiredDataGenerated() and requiredDataRemote() methods to determine all
 * the required data, the data required by the module from the pipeline, and
 * the data that must be obtained from the client, respectively.
 *
 * To define the module's functionality, implement the run() method.
 * The hash of data is passed as an argument to run(), where pointers to the
 * required data blobs are made available using a name lookup mechanism.
 */
class AbstractModule
{
    private:
        /// The service data and stream data required from the data client.
        DataRequirements _reqDataRemote;

        /// The data required that is generated in the pipeline (or module).
        DataRequirements _reqDataGenerated;

        /// The configuration node for the module.
        const ConfigNode* _config;

    protected:
        /// Adds the set of generated data as a requirement.
        /// The given \p list of data generated within the pipeline
        /// is added to the set of data required by the module.
        void addGeneratedData(const QSet<QString>& list) {
            _reqDataGenerated.addStreamData(list);
        }

        /// Adds the given generated data as a requirement.
        /// The given \p type of data generated within the pipeline
        /// is added to the set of data required by the module.
        void addGeneratedData(const QString& type) {
            _reqDataGenerated.addStreamData(type);
        }

        /// Adds the required service data set.
        /// The given \p list of service data supplied by the data client
        /// is added to the set of data required by the module.
        void addServiceData(const QSet<QString>& list) {
            _reqDataRemote.addServiceData(list);
        }

        /// Adds the given service data as a requirement.
        /// The given \p type of service data supplied by the data client
        /// is added to the set of data required by the module.
        void addServiceData(const QString& type) {
            _reqDataRemote.addServiceData(type);
        }

        /// Adds the required stream data set.
        /// The given \p list of stream data supplied by the data client
        /// is added to the set of data required by the module.
        void addStreamData(const QSet<QString>& list) {
            _reqDataRemote.addStreamData(list);
        }

        /// Adds the given stream data as a requirement.
        /// The given \p type of stream data supplied by the data client
        /// is added to the set of data required by the module.
        void addStreamData(const QString& type) {
            _reqDataRemote.addStreamData(type);
        }

    public:
        /// Creates a new abstract Pelican module with the given configuration.
        AbstractModule(const ConfigNode& config) {
            _config = &config;
        }

        /// Destroys the module (virtual).
        virtual ~AbstractModule() {}

        /// Returns all the data required by this module.
        /// This method returns a combination of all data required by
        /// the module (generated both within the pipeline, and supplied
        /// by the data client).
        DataRequirements requiredDataAll() const {
            return _reqDataRemote + _reqDataGenerated;
        }

        /// Returns the generated data required by this module.
        /// This method returns the data required by the module that is
        /// generated within the pipeline, including any data produced by
        /// the module itself.
        const DataRequirements& requiredDataGenerated() const {
            return _reqDataGenerated;
        }

        /// Returns the service data and stream data required by the module.
        /// This method returns the data required by the module that is
        /// provided by the data client, external to the pipeline.
        const DataRequirements& requiredDataRemote() const {
            return _reqDataRemote;
        }

        /// Runs the module (pure virtual).
        /// This public method is called by the pipeline to run the module.
        /// A reference to the complete hash of data blobs required for this iteration
        /// of the pipeline is passed as a function argument.
        ///
        /// The module updates the contents of this hash while running.
        ///
        /// @param[in,out] data The hash of data blobs required to run the pipeline
        ///                     module. The module updates the contents of the data
        ///                     blobs in the hash as required.
        virtual void run(QHash<QString, DataBlob*>& data) = 0;
};

} // namespace pelican

#endif // ABSTRACTMODULE_H 
