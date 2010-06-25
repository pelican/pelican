#include "pelican/comms/DataBlobResponse.h"
#include "pelican/utility/memCheck.h"

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

/**
 * @details
 */
DataBlobResponse::~DataBlobResponse()
{
}

} // namespace pelican
