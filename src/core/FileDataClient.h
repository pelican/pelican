#ifndef FILEDATACLIENT_H
#define FILEDATACLIENT_H

#include "core/AbstractDataClient.h"
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
 * Data client used by the pipeline for accessing data directly from a data
 * file rather than using the data server.
 * 
 * @details
 * This data client fetches data directly from one or more data files and
 * makes it available to the pipelines via the pipeline driver.
  */
class FileDataClient : public AbstractDataClient
{
    private:
        QString _fileVisibilities;  ///< Visibility data file name.
        QString _fileAntennas;      ///< Antenna positions file name.
        QString _fileChannels;      ///< Channel-to-frequency file name.

    public:
        /// Data client constructor.
        FileDataClient(const QDomElement& config,
                DataBlobFactory* blobFactory);

        /// Data client destructor
        ~FileDataClient();

        /// Gets the requested data.
        QHash<QString, DataBlob*>& getData(const DataRequirements&);
};

} // namespace pelican

#endif // FILEDATACLIENT_H
