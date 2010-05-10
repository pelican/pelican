#ifndef DATABLOBRESPONSE_H
#define DATABLOBRESPONSE_H


#include "ServerResponse.h"

/**
 * @file DataBlobResponse.h
 */

namespace pelican {

/**
 * @class DataBlobResponse
 *  
 * @brief
 *    A reponse from the server that contains a DataBlob
 * @details
 * 
 */

class DataBlobResponse : public ServerResponse
{

    public:
        DataBlobResponse( const QString& blobType, const QString& dataName );
        ~DataBlobResponse();
        QString blobClass() { return _type; }
        QString dataName() { return _name; }

    private:
        QString _type;
        QString _name;
};

} // namespace pelican
#endif // DATABLOBRESPONSE_H 
