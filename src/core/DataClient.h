#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <QHash>
#include <QString>

/**
 * @file DataClient.h
 */

namespace pelican {

class DataBlob;
class DataRequirements;

/**
 * @class DataClient
 *  
 * @brief
 * This is the data client abstract base class for the pipeline driver.
 * 
 * @details
 * The data client fetches data from the data server and makes it available
 * to the pipelines via the pipeline driver. The PipelineApplication creates
 * the appropriate data client object based on the supplied configuration.
 *
 * Inherit this class to create a new data client type.
 */
class DataClient
{
    public:
        /// Data client constructor.
        DataClient();

        /// Data client destructor (virtual).
        virtual ~DataClient();

        /// Gets the requested data from the data server.
        virtual QHash<QString, DataBlob*> getData(const DataRequirements&) = 0;
};

} // namespace pelican

#endif // DATACLIENT_H 
