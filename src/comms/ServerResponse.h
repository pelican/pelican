#ifndef SERVERRESPONSE_H
#define SERVERRESPONSE_H
#include <QString>

/**
 * @file ServerResponse.h
 */

namespace pelican {

/**
 * @class ServerResponse
 *  
 * @brief
 *    Base class to manage all responses from the pelican server
 * @details
 * 
 */

class ServerResponse
{
    public:
        enum private_Response_t { Error, Acknowledge, StreamData, ServiceData };
        typedef private_Response_t Response_t;

    public:
        ServerResponse(Response_t type = Error, const QString& msg = "" );
        ~ServerResponse();
        Response_t type() const;

    private:
        Response_t _type;
        QString _msg;
};

} // namespace pelican
#endif // SERVERRESPONSE_H 
