#include "AbstractDataBlobClient.h"

#include <QtCore/QSet>
#include <iostream>

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
    if( level <= _verbose )
    {
        std::cout << "DataBlobClient: " << msg.toStdString() << std::endl;
    }
}

void AbstractDataBlobClient::subscribe(const QString& stream)
{
    QSet<QString> set;
    set.insert(stream);
    subscribe(set);
}

} // namespace pelican
