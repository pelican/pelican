#include "DataBlobVerify.h"
#include "pelican/data/DataBlob.h"
#include "pelican/data/DataBlobFactory.h"
#include <QtCore/QDebug>


namespace pelican {


/**
 *@details DataBlobVerify 
 */
DataBlobVerify::DataBlobVerify( const DataBlob* blob )
    : _blob(blob)
{
    _buffer.open(QBuffer::WriteOnly);
}

/**
 *@details
 */
DataBlobVerify::~DataBlobVerify()
{
}

bool DataBlobVerify::verifySerialisedBytes() const {
    int size=_blob->serialisedBytes();
    _serialise();
    return size == _buffer.size();
}

bool DataBlobVerify::verifyDeserialise() const {
    _serialise();
    static FactoryGeneric<DataBlob> factory(true);
    DataBlob* copy = factory.create( _blob->type() );
    QByteArray tmp( _buffer.data() );
    QBuffer buffer2( &tmp );
    buffer2.open( QBuffer::ReadOnly );
    copy->deserialise( buffer2, QSysInfo::ByteOrder );
    QBuffer copyBuffer;
    copyBuffer.open(QBuffer::WriteOnly);
    copy->serialise( copyBuffer );
    if( copyBuffer.buffer().size() == 0 ) return false;
    return copyBuffer.buffer() == _buffer.buffer();
}

void DataBlobVerify::_serialise() const {
    if( _buffer.size() == 0 ) {
        _blob->serialise( _buffer );
    }
}

} // namespace pelican
