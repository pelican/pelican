#ifndef DATABLOBRESPONSE_H
#define DATABLOBRESPONSE_H

/**
 * @file DataBlobResponse.h
 */

#include <QtCore/QSysInfo>
#include "ServerResponse.h"

namespace pelican {

/**
 * @ingroup c_comms
 *
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
        DataBlobResponse( QDataStream& stream );
        ~DataBlobResponse();
        const QString& blobClass() const { return _type; }
        const QString& dataName() const { return _name; }
        quint64 dataSize() const {return _dataSize;}
        QSysInfo::Endian byteOrder() const {return _endianness;}

        // serialise and deserialise important data to/from a stream
        void serialise( QDataStream& stream );
        void deserialise( QDataStream& stream );
        static size_t serialisedSize();

    private:
        QString _type;
        QString _name;
        quint64 _dataSize;
        QSysInfo::Endian _endianness;
};

} // namespace pelican

#endif // DATABLOBRESPONSE_H
