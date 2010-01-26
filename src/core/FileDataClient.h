#ifndef FILEDATACLIENT_H
#define FILEDATACLIENT_H

#include <QHash>
#include <QString>

/**
 * @file FileDataClient.h
 */

namespace pelican {

class DataBlob;
class DataRequirements;

/**
 * @class FileDataClient
 *  
 * @brief
 * Data client used by the pipeline for accessing data directly from a data files
 * rather than using the data server.
 * 
 * @details
 * This data client fetches data directly from one or more data files and
 * makes it available to the pipelines via the pipeline driver. The
 * PipelineApplication creates the appropriate data client object based on
 * the supplied configuration.
  */

class FileDataClient : public AbstractDataClient
{
    public:
        /// Data client constructor.
        FileDataClient();

        /// Data client destructor
        ~FileDataClient();

        /// Gets the requested data from the data server.
        QHash<QString, DataBlob*> getData(const DataRequirements&);

    private:
        QVector<DataBlob> _streamData;
        QVector<DataBlob> _serviceData;
};

} // namespace pelican

#endif // FILEDATACLIENT_H
