#include "Stream.h"


#include "pelican/data/DataBlob.h"
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details Constructs a Stream object.
 */
Stream::Stream(const QString& streamName)
    : _name(streamName)
{
}

/**
 * @details Destroys the Stream object.
 */
Stream::~Stream()
{
}

boost::shared_ptr<DataBlob> Stream::data() const
{
    return _data;
}

const QString& Stream::type() const { 
    return _type; 
}

void Stream::setData(DataBlob* blob)
{
    _data.reset(blob);
    _type = blob->type();
}


} // namespace pelican
