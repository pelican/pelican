#ifndef ABSTRACT_DATA_CLIENT_H
#define ABSTRACT_DATA_CLIENT_H

/**
 * @file AbstractDataClient.h
 */

#include "pelican/core/DataTypes.h"
#include "pelican/utility/FactoryRegistrar.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/FactoryConfig.h"

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QSet>
#include <QtCore/QString>

class QIODevice;

namespace pelican {

// This macro is used to register the named client type.
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
 * Abstract base class for all data clients.
 *
 * @details
 * Data clients fetch data in order to make it available to the pipelines via
 * the PipelineDriver. The data client is created by the pipeline driver
 * after determining the data requirements of all of the pipelines.
 *
 * In order to create a data client, inherit this class and implement the
 * getData() method.
 *
 * \par Common configuration:
 *
 * Under its root XML configuration node, each data client must have a list of
 * data types and their associated adapters. This information is then used
 * by the data client factory for automatically creating the adapters attached
 * to each data type.
 *
 * For example:
 *
 * \code
 *  <data type="VisibilityData" adapter="AdapterStationVisibilities"/>
 *  <data type="AntennaPositions" adapter="AdapterStationAntennas"/>
 * \endcode
 */

class AbstractDataClient
{
    public:
        typedef QHash<QString, DataBlob*> DataBlobHash;

    public:

        /// Data client constructor.
        AbstractDataClient(const ConfigNode& configNode, const DataTypes& types,
                           const Config* config
                );

        /// Data client destructor.
        virtual ~AbstractDataClient();

        PELICAN_CONSTRUCT_TYPES(const ConfigNode&, const DataTypes&, const Config*)

    public:
        /// Retrieves data requested by pipelines and fills the given data hash,
        /// and returns another hash of valid data.
        /// (\todo what is valid data in this context?)
        virtual DataBlobHash getData(DataBlobHash&) = 0;

        /// Returns the list of data requirements for each pipeline.
        const QList<DataRequirements>& dataRequirements() { return _dataRequirements; }


    protected:
        /// Writes a message to the log.
        void log(const QString& msg);

        /// Returns a pointer to the configuration node.
        const ConfigNode& configNode() const {return _configNode;}

/*
        /// Adapts (de-serialises) stream data.
        DataBlobHash adaptStream(QIODevice& device, const StreamData* d,
                DataBlobHash& dataHash);

        /// Adapts (de-serialises) service data.
        DataBlobHash adaptService(QIODevice& device, const DataChunk* sd,
                DataBlobHash& dataHash);


        /// Returns the adapter for service data of the required type.
        AbstractServiceAdapter* serviceAdapter(const QString& type) const
        { return _dataReqs.serviceAdapter(type); }

        /// Returns the adapter for stream data of the required type.
        AbstractStreamAdapter* streamAdapter(const QString& type) const
        { return _dataReqs.streamAdapter(type); }
*/

    protected:
        ConfigNode _configNode; ///< The configuration node for the data client.
        QList<DataRequirements> _dataRequirements;
        //DataTypes _dataReqs;    ///< The DataTypes and requirements.
        const Config* _config;
        QSet<QString> _requireSet;
};

} // namespace pelican
#endif // ABSTRACT_DATA_CLIENT_H
