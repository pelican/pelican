#ifndef ABSTRACTDATABLOBCLIENT_H
#define ABSTRACTDATABLOBCLIENT_H

#include <QObject>

/**
 * @file AbstractDataBlobClient.h
 */

namespace pelican {
    class DataBlob;
    class Stream;

/**
 * @class AbstractDataBlobClient
 * 
 * @brief
 *   Base class for all clients of the 
 *   Pelican DataBlob server
 * @details
 * 
 */
class AbstractDataBlobClient : public QObject
{
    Q_OBJECT

    public:
        /// AbstractDataBlobClient constructor.
        AbstractDataBlobClient(QObject* parent=0);

        /// AbstractDataBlobClient destructor.
        virtual ~AbstractDataBlobClient();

        /// returns the streams served by the blob server
        virtual QSet<QString> streams() = 0;

    protected:
        /// report verbose messages
        void verbose(const QString&, int level = 1);

    signals:
        void newData(const Stream& stream);
        void newStreamsAvailable();

    protected:
        int _verbose;
};

} // namespace pelican

#endif // ABSTRACTDATABLOBCLIENT_H 
