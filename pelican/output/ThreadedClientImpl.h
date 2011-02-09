#ifndef THREADEDCLIENTIMPL_H
#define THREADEDCLIENTIMPL_H

/**
 * @file ThreadedClientImpl.h
 */

#include "pelican/utility/ConfigNode.h"
#include <QtCore/QThread>

namespace pelican {
    class DataBlobClient;

/**
 * @ingroup c_output
 *
 * @class ThreadedClientImpl
 *
 * @brief
 *    Implentation details for the ThreadeDataBlobClient class
 * @details
 *
 */
class ThreadedClientImpl : public QThread
{
    Q_OBJECT

    public:
        /// ThreadedClientImpl constructor.
        ThreadedClientImpl( const ConfigNode& c, QObject* parent=0);

        /// ThreadedClientImpl destructor.
        ~ThreadedClientImpl();

        // return the lient
        DataBlobClient* client() { return _client; };

    protected:
        void run();

    private:
        DataBlobClient* _client;
        ConfigNode _config;
};

} // namespace pelican

#endif // THREADEDCLIENTIMPL_H
