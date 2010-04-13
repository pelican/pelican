#ifndef FILEDATACLIENT_H
#define FILEDATACLIENT_H

#include "pelican/core/AbstractDataClient.h"
#include <QHash>
#include <QString>

/**
 * @file FileDataClient.h
 */

namespace pelican {

class DataBlob;
class ConfigNode;
class DataTypes;

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
        QHash<QString, QString> _fileNames; ///< Hash of filenames for each data type.

    public:
        /// Data client constructor.
        FileDataClient(const ConfigNode& config, const DataTypes& types );

        /// Data client destructor
        ~FileDataClient();

        /// Gets the requested data.
        QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>&);

    private:
        /// Gets the configuration options.
        void _getConfig();
};

} // namespace pelican

#endif // FILEDATACLIENT_H
