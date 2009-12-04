#include "ServerRequest.h"
#include "utility/memCheck.h"


namespace pelican {


// class ServerRequest 
ServerRequest::ServerRequest(const QString& type)
{
    setType(type);
}

ServerRequest::ServerRequest(const QStringList& tokens)
{
    if( ! tokens.isEmpty() )
        setType(tokens.first());
}

ServerRequest::~ServerRequest()
{
}

const QString& ServerRequest::type() const
{
    return _type;
}

void ServerRequest::setType(const QString& type)
{
    _type = type;
}


bool ServerRequest::error() const
{
    return false;
}

QString ServerRequest::errorMessage() const
{
    return _error;
}
} // namespace pelican
