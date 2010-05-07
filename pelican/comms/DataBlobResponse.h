#ifndef DATABLOBRESPONSE_H
#define DATABLOBRESPONSE_H


#include "ServerResponse.h"
#include "pelican/data/DataBlob.h"

/**
 * @file DataBlobResponse.h
 */

namespace pelican {
//    class DataBlob;

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
        DataBlobResponse( DataBlob* blob );
        ~DataBlobResponse();

        DataBlob* dataBlob() const { return _data; };

        /// fetch the DataBlob object returned in the request
        template<class T> 
            T* getBlob() const {
            return static_cast<T>(_data);
        }

    private:
        DataBlob* _data;
};

} // namespace pelican
#endif // DATABLOBRESPONSE_H 
