#include "ThreadedDataBlobClient.h"
#include <QtCore/QSet>

#include "pelican/output/ThreadedClientImpl.h"
#include "pelican/output/DataBlobClient.h"

namespace pelican {

/**
 * @details Constructs a ThreadedDataBlobClient object.
 */
ThreadedDataBlobClient::ThreadedDataBlobClient(const ConfigNode& c)
    : AbstractDataBlobClient()
{
    _thread = new ThreadedClientImpl(c);
    _thread->start();
    while( ! _thread->client() ) { sleep(1); } // wait until the client exists
    connect( _thread->client(), SIGNAL(newData(const Stream&)),
                        this, SLOT( emitNewData(const Stream&) ), Qt::DirectConnection );
    connect( _thread->client(), SIGNAL(newStreamsAvailable()),
            this, SLOT( emitNewStreamsAvailable() ), Qt::DirectConnection );
}

/**
 * @details Destroys the ThreadedDataBlobClient object.
 */
ThreadedDataBlobClient::~ThreadedDataBlobClient()
{
    do { _thread->quit(); } while( ! _thread->wait(10) );
    delete _thread;
}

QSet<QString> ThreadedDataBlobClient::streams()
{
    if( _thread->client() )
        return _thread->client()->streams();
    QSet<QString> s;
    return s;
}

void ThreadedDataBlobClient::emitNewStreamsAvailable()
{
    emit newStreamsAvailable();
}

void ThreadedDataBlobClient::emitNewData(const Stream& s)
{
    emit newData(s);
}


/// set the host to listen to
void ThreadedDataBlobClient::setHost(const QString& host)
{
    _thread->client()->setHost(host);
}

/// set the port to listen on
void ThreadedDataBlobClient::setPort(quint16 port)
{
    _thread->client()->setPort(port);
}

void ThreadedDataBlobClient::subscribe( const QSet<QString>& streams )
{
    _thread->client()->subscribe(streams);
}

} // namespace pelican
