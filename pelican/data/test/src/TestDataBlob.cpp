#include "pelican/data/test/TestDataBlob.h"

#include <QtCore/QDataStream>


namespace pelican {


// class TestDataBlob 
TestDataBlob::TestDataBlob()
    : DataBlob("TestDataBlob")
{
}

TestDataBlob::~TestDataBlob()
{
}

void TestDataBlob::setData(const QString& data)
{
    _data = data;
}

bool TestDataBlob::operator==(const TestDataBlob& blob)
{
    return _data == blob._data;
}

void TestDataBlob::serialise(QIODevice& device) const
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << _data;
    device.write( array );
}

void TestDataBlob::deserialise(QIODevice& device)
{
    QDataStream in(&device);
    in.setVersion(QDataStream::Qt_4_0);
    in >> _data;
}

} // namespace pelican

