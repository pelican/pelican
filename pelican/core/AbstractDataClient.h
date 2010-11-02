#ifndef ABSTRACTDATACLIENT_H
#define ABSTRACTDATACLIENT_H

/**
 * @file AbstractDataClient.h
 */

#include "pelican/core/DataTypes.h"
#include "pelican/utility/FactoryRegistrar.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QString>

class QIODevice;


namespace pelican {

/**
 * This macro is used to register the named client type.
 */
#define PELICAN_DECLARE_CLIENT(type) PELICAN_DECLARE(AbstractDataClient, type)

class AbstractAdapter;
class DataBlob;
class StreamData;
class DataChunk;

/**
 * @ingroup c_core
 *
 * @class AbstractDataClient
 *
 * @brief
 * This is the abstract base class for the data client used by the pipeline
 * driver.
 *
 * @details
 * The data client fetches data from the data server and makes it available
 * to the pipelines via the pipeline driver. The data client is created by
 * the pipeline driver after determining the data requirements.
 *
 * Inherit this class and implement the getData() method to create a new data
 * client type.
 *
 * \section Configuration
 *
 * Under its root XML configuration node, each data client must have a list of
 * data types and data adapters, for example:
 *
 * \verbatim
      <data type="VisibilityData" adapter="AdapterStationVisibilities"/>
      <data type="AntennaPositions" adapter="AdapterStationAntennas"/>
 * \endverbatim
 *
 * This information is read by the abstract data client, and stored in the
 * hash accessible using the adapterNames() method.
 */
class AbstractDataClient
{
    private:
        /// The configuration node for the data client.
        ConfigNode _configNode;

        /// The DataTypes and requirements.
        DataTypes _dataReqs;

    protected:
        const Config* _config;
        QSet<QString> _requireSet;

    protected: // methods
        /// Writes a message to the log.
        void log(const QString& msg);

        /// Adapts stream data.
        QHash<QString, DataBlob*> adaptStream(QIODevice& device,
                const StreamData* sd, QHash<QString, DataBlob*>& dataHash );

        /// Adapts service data.
        QHash<QString, DataBlob*> adaptService(QIODevice& device,
                const DataChunk* d, QHash<QString, DataBlob*>& dataHash );

        /// Returns a pointer to the configuration node.
        const ConfigNode& configNode() const {return _configNode;}

        /// Returns the adapter for service data of the required type.
        AbstractServiceAdapter* serviceAdapter(const QString& type) const {
            return _dataReqs.serviceAdapter(type);
        }

        /// Returns the adapter for stream data of the required type.
        AbstractStreamAdapter* streamAdapter(const QString& type) const {
            return _dataReqs.streamAdapter(type);
        }

    public:
        /// Data client constructor.
        PELICAN_CONSTRUCT_TYPES(const ConfigNode&, const DataTypes&, const Config*)
        AbstractDataClient(const ConfigNode& configNode,
                const DataTypes& types, const Config* config);

        /// Data client destructor (virtual).
        virtual ~AbstractDataClient();

        /// Returns the list of data requirements for each pipeline.
        const QList<DataRequirements>& dataRequirements() {
            return _dataReqs.dataRequirements();
        }

        /// returns the type of data associated with the data name (stream/service/unknown)
        AbstractAdapter::AdapterType_t type(const QString& dataName) const {
            return _dataReqs.type(dataName);
        }

        /// Gets the requested data from the data server.
        /// This method gets tFills the given data hash, and returns another hash of valid data.
        virtual QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>&) = 0;
};

} // namespace pelican

#endif // ABSTRACTDATACLIENT_H
