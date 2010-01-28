#ifndef ABSTRACTDATACLIENT_H
#define ABSTRACTDATACLIENT_H

#include <QHash>
#include <QString>
#include <QDataStream>

class QIODevice;

/**
 * @file AbstractDataClient.h
 */

namespace pelican {

class DataBlob;
class DataRequirements;

/**
 * @class AbstractDataClient
 *  
 * @brief
 * This is the abstract base class for the data client used by the pipeline
 * driver.
 * 
 * @details
 * The data client fetches data from the data server and makes it available
 * to the pipelines via the pipeline driver. The PipelineApplication creates
 * the appropriate data client object based on the supplied configuration.
 *
 * Inherit this class and implement the getData() method to create a new data
 * client type.
 */
class AbstractDataClient
{
    private:
        QDataStream _inputDataStream;

    public:
        /// Data client constructor.
        AbstractDataClient();

        /// Data client destructor (virtual).
        virtual ~AbstractDataClient();

        /// Gets the requested data from the data server.
        virtual QHash<QString, DataBlob*> getData(const DataRequirements&) = 0;

        /// Sets the input device.
        void setDevice(QIODevice *device);
};

} // namespace pelican

#endif // ABSTRACTDATACLIENT_H
