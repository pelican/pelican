#include "DataBlobAdapter.h"
#include "pelican/data/DataBlob.h"
#include "pelican/comms/DataBlobResponse.h"
#include <QtCore/QDataStream>
#include <QtCore/QSysInfo>


namespace pelican {


/**
 *@details DataBlobAdapter 
 */
DataBlobAdapter::DataBlobAdapter(const ConfigNode& config)
    : AbstractAdapter( AbstractAdapter::Service, config )
{
}

/**
 *@details
 */
DataBlobAdapter::~DataBlobAdapter()
{
}

void DataBlobAdapter::deserialise(QIODevice* in) {
    QDataStream stream( in );
    stream.setVersion(QDataStream::Qt_4_0);
    // get the meta information
    DataBlobResponse res( stream );
    // now desearlise the blob
    dataBlob()->deserialise( *in, res.byteOrder() );
}

} // namespace pelican
