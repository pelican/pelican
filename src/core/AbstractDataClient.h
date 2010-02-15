#ifndef ABSTRACTDATACLIENT_H
#define ABSTRACTDATACLIENT_H

#include <QHash>
#include <QString>
#include <QDataStream>
#include <QDomElement>

/**
 * @file AbstractDataClient.h
 */

namespace pelican {

class DataBlob;
class DataRequirements;
class DataBlobFactory;
class ConfigNode;

/**
 * @class AbstractDataClient
 *  
 * @brief
 * This is the abstract base class for the data client used by the pipeline
 * driver.
 * 
 * @details
 * The data client fetches data from the data server and makes it available
 * to the pipelines via the pipeline driver. The pipeline application defines
 * the appropriate data client object in main(), and the PipelineApplication
 * class is responsible for creating it.
 *
 * Inherit this class and implement the getData() method to create a new data
 * client type.
 */
class AbstractDataClient
{
    private:
        /// The configuration node for the data client.
        const ConfigNode* _config;

    public:
        /// Data client constructor.
        AbstractDataClient(const ConfigNode& config);

        /// Data client destructor (virtual).
        virtual ~AbstractDataClient();

        /// Gets the requested data from the data server.
        virtual void getData(const DataRequirements&, QHash<QString, DataBlob*>&) = 0;
};

} // namespace pelican

#endif // ABSTRACTDATACLIENT_H
