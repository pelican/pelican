#include "TestDataBlobClient.h"


#include "pelican/output/Stream.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details Constructs a TestDataBlobClient object.
 */
TestDataBlobClient::TestDataBlobClient()
{
}

/**
 * @details Destroys the TestDataBlobClient object.
 */
TestDataBlobClient::~TestDataBlobClient()
{
}

void TestDataBlobClient::send(const Stream& s)
{
    if( ! _streams.contains( s.name() ) )
    {
        _streams.insert( s.name() );
        emit newStreamsAvailable();
    }
    emit newData(s);
}

QSet<QString> TestDataBlobClient::streams()
{
    return _streams;
}

void TestDataBlobClient::subscribe( const QSet<QString>& streams )
{
    _subs.unite(streams);
}

const QSet<QString>& TestDataBlobClient::subscriptions() const
{
    return _subs;
}

} // namespace pelican
