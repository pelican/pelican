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
        QDomElement _config;

        /// A pointer to the data blob factory.
        DataBlobFactory* _blobFactory;

    protected: /* Data */
        /// The hash of data returned by the getData() method.
        QHash<QString, DataBlob*> _dataHash;

    protected: /* Methods */
        /// Returns a pointer to the data blob factory.
        DataBlobFactory* blobFactory() {return _blobFactory;}

    public:
        /// Data client constructor.
        AbstractDataClient(const QDomElement& config,
                DataBlobFactory* blobFactory);

        /// Data client destructor (virtual).
        virtual ~AbstractDataClient();

        /// Gets the requested data from the data server.
        virtual QHash<QString, DataBlob*>& getData(const DataRequirements&) = 0;

        /// Returns a configuration option (attribute).
        QString getOption(const QString& tagName, const QString& attribute,
                const QString& defValue = QString());
};

} // namespace pelican

#endif // ABSTRACTDATACLIENT_H
