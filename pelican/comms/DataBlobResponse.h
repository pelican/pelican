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
 * A response from the server that contains a DataBlob
 *
 * @details
 * 
 */
class DataBlobResponse : public ServerResponse
{
    public:
        DataBlobResponse( const QString& blobType, const QString& dataName,
                quint64 dataSize, QSysInfo::Endian endianness );
        ~DataBlobResponse();
        const QString& blobClass() const { return _type; }
        const QString& dataName() const { return _name; }
        quint64 dataSize() const {return _dataSize;}
        QSysInfo::Endian byteOrder() const {return _endianness;}

    private:
        QString _type;
        QString _name;
        quint64 _dataSize;
        QSysInfo::Endian _endianness;
};

} // namespace pelican

#endif // DATABLOBRESPONSE_H 
