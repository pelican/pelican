#include "AbstractDataBlobClient.h"

#include <iostream>
#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details Constructs a AbstractDataBlobClient object.
 */
AbstractDataBlobClient::AbstractDataBlobClient(QObject* parent)
    : QObject(parent), _verbose(0)
{
}

/**
 * @details Destroys the AbstractDataBlobClient object.
 */
AbstractDataBlobClient::~AbstractDataBlobClient()
{
}

void AbstractDataBlobClient::verbose(const QString& msg, int level)
{
    if( level > _verbose )
    {
        std::cout << "DataBlobClient: " << msg.toStdString() << std::endl;
    }
}

} // namespace pelican
