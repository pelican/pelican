#include "AbstractOutputStream.h"


#include <iostream>
#include "pelican/utility/ConfigNode.h"

namespace pelican {


/**
 *@details AbstractOutputStream
 */
AbstractOutputStream::AbstractOutputStream( const ConfigNode& c )
    : _verbose(0)
{
    if( c.hasAttribute("verbose") )
        _verbose = c.getAttribute("verbose").toInt();
}

/**
 *@details
 */
AbstractOutputStream::~AbstractOutputStream()
{
}

void AbstractOutputStream::send(const QString& streamName, const DataBlob* dataBlob)
{
    verbose( QString("sending stream \"") + streamName + "\"");
    sendStream(streamName, dataBlob );
}


void AbstractOutputStream::verbose(const QString& msg, int level)
{
    if( level <= _verbose )
    {
        std::cout << "OutputStream: " << msg.toStdString() << std::endl;
    }
}

} // namespace pelican
