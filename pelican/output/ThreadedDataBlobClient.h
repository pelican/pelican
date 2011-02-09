#ifndef THREADEDDATABLOBCLIENT_H
#define THREADEDDATABLOBCLIENT_H

#include <QtCore/QThread>
#include "AbstractDataBlobClient.h"
#include "pelican/utility/ConfigNode.h"

/**
 * @file ThreadedDataBlobClient.h
 */

namespace pelican {
    class ThreadedClientImpl;

/**
 * @ingroup c_output
 *
 * @class ThreadedDataBlobClient
 *
 * @brief
 *    A DataBlobClient that operates in its own thread
 * @details
 *
 */
class ThreadedDataBlobClient : public AbstractDataBlobClient
{
    Q_OBJECT

    public:
        /// ThreadedDataBlobClient constructor.
        ThreadedDataBlobClient(const ConfigNode& c);

        /// ThreadedDataBlobClient destructor.
        ~ThreadedDataBlobClient();

        /// set the host to listen to
        void setHost(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

        // base class interface requirements
        virtual QSet<QString> streams();
        virtual void subscribe( const QSet<QString>& streams );

    protected slots:
        void emitNewStreamsAvailable();
        void emitNewData(const Stream&);

    private:
        ThreadedClientImpl* _thread;
        ConfigNode _config;
};

} // namespace pelican

#endif // THREADEDDATABLOBCLIENT_H
