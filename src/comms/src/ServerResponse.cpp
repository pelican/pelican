#include "ServerResponse.h"


#include "utility/memCheck.h"

namespace pelican {


// class ServerResponse 
ServerResponse::ServerResponse(ServerResponse::Response_t type)
    : _type(type)
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
