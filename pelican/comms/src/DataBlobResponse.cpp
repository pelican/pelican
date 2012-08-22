#include "pelican/comms/DataBlobResponse.h"
#include <QtCore/QDataStream>

namespace pelican {

/**
 * @details DataBlobResponse
 */
DataBlobResponse::DataBlobResponse(const QString& blobType,
        const QString& streamName,
        quint64 dataSize, QSysInfo::Endian endianness )
    : ServerResponse( ServerResponse::Blob ),
      _type(blobType),
      _name(streamName),
      _dataSize(dataSize),
      _endianness(endianness)
{
}

DataBlobResponse::DataBlobResponse( QDataStream& stream ) 
    : ServerResponse( ServerResponse::Blob )
{
    deserialise(stream);
}

/**
 * @details
 */
DataBlobResponse::~DataBlobResponse()
{
}

void DataBlobResponse::serialise( QDataStream& stream ) {
    stream <<  _type << _name;
    stream << _dataSize;
    stream << (int)_endianness;
}

void DataBlobResponse::deserialise( QDataStream& stream ) {
    stream >> _type >> _name;
    stream >> _dataSize;
    int tmp;
    stream >> tmp;
    _endianness = (QSysInfo::Endian)tmp;
}

size_t DataBlobResponse::serialisedSize() {
    return sizeof(_type) + sizeof(_name) + sizeof(_dataSize) + sizeof(_endianness);
}
} // namespace pelican
