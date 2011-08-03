#include "ThreadedClientImpl.h"


#include "pelican/output/DataBlobClient.h"

namespace pelican {


/**
 * @details Constructs a ThreadedClientImpl object.
 */
ThreadedClientImpl::ThreadedClientImpl(const ConfigNode& c, QObject* parent)
    : QThread(parent), _client(0), _config(c)
{
}

/**
 * @details Destroys the ThreadedClientImpl object.
 */
ThreadedClientImpl::~ThreadedClientImpl()
{
}

void ThreadedClientImpl::run()
{
    _client = new DataBlobClient(_config);
    exec();
    delete _client;
    _client = 0;
}


} // namespace pelican
