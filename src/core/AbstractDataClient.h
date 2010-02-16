#ifndef ABSTRACTDATACLIENT_H
#define ABSTRACTDATACLIENT_H

#include "data/DataRequirements.h"
#include <QHash>
#include <QList>
#include <QString>

/**
 * @file AbstractDataClient.h
 */

namespace pelican {

class AdapterFactory;
class AbstractAdapter;
class ConfigNode;
class DataBlob;

/**
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
 *
 *
 * \section Configuration
 *
 * Under its root XML configuration node, each data client must have a list of
 * data types and data adapters, for example:
 *
 * \verbatim
 *     <data type="VisibilityData" adapter="AdapterStationVisibilities"/>
 *     <data type="AntennaPositions" adapter="AdapterStationAntennas"/>
 * \endverbatim
 *
 * This information is read by the abstract data client, and stored in the
 * hash accessible using the adapterNames() method.
 */
class AbstractDataClient
{
    private:
        /// The configuration node for the data client.
        const ConfigNode* _configNode;

        /// Pointer to the adapter factory.
        AdapterFactory* _adapterFactory;

        /// List of data requirements for each pipeline.
        QList<DataRequirements> _dataRequirements;

        /// All the adapters created for each data type and pipeline.
        QList<QHash<QString, AbstractAdapter*> > _adapters;

        /// The adapter names required for each data type.
        QHash<QString, QString> _adapterNames;

    protected:
        /// Returns a pointer to the configuration node.
        const ConfigNode* configNode() {return _configNode;}

        /// Returns a pointer to the adapter factory.
        AdapterFactory* adapterFactory() {return _adapterFactory;}

        /// Returns the list of data requirements for each pipeline.
        QList<DataRequirements>& dataRequirements() {return _dataRequirements;}

        /// Returns the list of adapters for each data type and each pipeline.
        QList<QHash<QString, AbstractAdapter*> >& adapters() {return _adapters;}

        /// Returns the adapter names required for each data type.
        QHash<QString, QString> adapterNames() {return _adapterNames;}

    public:
        /// Data client constructor.
        AbstractDataClient(const ConfigNode& config,
                AdapterFactory* adapterFactory,
                QList<DataRequirements>& dataRequirements);

        /// Data client destructor (virtual).
        virtual ~AbstractDataClient();

        /// Gets the requested data from the data server.
        /// This method gets tFills the given data hash, and returns another hash of valid data.
        virtual QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>&) = 0;
};

} // namespace pelican

#endif // ABSTRACTDATACLIENT_H
