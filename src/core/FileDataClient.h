#ifndef FILEDATACLIENT_H
#define FILEDATACLIENT_H

#include "core/AbstractDataClient.h"
#include <QHash>
#include <QString>
#include <QVector>

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
        QVector<DataBlob*> _streamData;
        QVector<DataBlob*> _serviceData;

        QString _fileVisibilities;  ///< Visibility data file name.
        QString _fileAntennas;      ///< Antenna positions file name.
        QString _fileChannels;      ///< Channel-to-frequency file name.

        /* Visibility file parameters */
        unsigned _nAntennas;        ///< Number of antennas.
        unsigned _nChannels;        ///< Number of frequency channels.
        unsigned _nPolarisations;   ///< Number of polarisations.

    public:
        /// Data client constructor.
        FileDataClient(const QDomElement& config);

        /// Data client destructor
        ~FileDataClient();

        /// Gets the requested data.
        QHash<QString, DataBlob*> getData(const DataRequirements&);

    private:
        /// Gets the visibility data from the file into the data blob.
        void _getVisibilityData(QString filename, unsigned nAntennas,
                unsigned nChannels, unsigned nPols);
};

} // namespace pelican

#endif // FILEDATACLIENT_H
