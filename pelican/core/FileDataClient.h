#ifndef FILEDATACLIENT_H
#define FILEDATACLIENT_H

/**
 * @file FileDataClient.h
 */

#include "pelican/core/AbstractDataClient.h"
#include <QtCore/QHash>
#include <QtCore/QString>

namespace pelican {

class DataBlob;
class ConfigNode;
class DataTypes;

/**
 * @ingroup c_core
 *
 * @class FileDataClient
 *
 * @brief
 * Data client used by the pipeline for accessing data directly from files
 * rather than using the data server.
 *
 * @details
 * This data client fetches data directly from one or more data files and
 * makes it available to the pipelines via the pipeline driver.
 */

class FileDataClient : public AbstractDataClient
{
    public:
        /// Data client constructor.
        FileDataClient(const ConfigNode& configNode,
                const DataTypes& types, const Config* config);

        /// Data client destructor
        virtual ~FileDataClient();

    public:
        /// Gets the requested data.
        DataBlobHash getData(DataBlobHash&);

    private:
        /// Reads the configuration options.
        void _getConfig();

    private:
        // Hash of filenames for each data type.
        QHash<QString, QString> _fileNames;
};

PELICAN_DECLARE_CLIENT(FileDataClient)

} // namespace pelican
#endif // FILEDATACLIENT_H
