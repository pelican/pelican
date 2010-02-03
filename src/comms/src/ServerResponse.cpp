#include "ServerResponse.h"


#include "utility/memCheck.h"

namespace pelican {


// class ServerResponse 
ServerResponse::ServerResponse(ServerResponse::Response_t type, const QString& msg)
    : _type(type), _msg(msg)
{
}

ServerResponse::~ServerResponse()
{
}

ServerResponse::Response_t ServerResponse::type() const
{
    return _type;
}

} // namespace pelican
