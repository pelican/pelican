#include "DataBlobAdapter.h"
#include "data/DataBlob.h"
#include "comms/DataBlobResponse.h"
#include <QtCore/QDataStream>
#include <QtCore/QSysInfo>


namespace pelican {


/**
 *@details DataBlobAdapter 
 */
DataBlobAdapter::DataBlobAdapter(const ConfigNode& config)
    : AbstractAdapter( config, AbstractAdapter::Unknown )
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
