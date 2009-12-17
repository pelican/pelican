#include "ServerRequest.h"
#include "utility/memCheck.h"


namespace pelican {


// class ServerRequest 
ServerRequest::ServerRequest(Request_t type, const QString& msg)
    : _type(type), _error(msg)
{
}

ServerRequest::~ServerRequest()
{
}

ServerRequest::Request_t ServerRequest::type() const
{
    return _type;
}

QString ServerRequest::message() const
{
    return _error;
}
} // namespace pelican
