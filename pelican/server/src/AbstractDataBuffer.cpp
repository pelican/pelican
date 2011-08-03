#include "AbstractDataBuffer.h"
#include <iostream>

namespace pelican {

void AbstractDataBuffer::verbose(const QString& msg, int level )
{
    if( _verboseLevel >= level )
        std::cout << _type.toStdString() << ": " << msg.toStdString() << std::endl;
}


} // namespace pelican
