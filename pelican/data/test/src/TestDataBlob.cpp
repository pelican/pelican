#include "TestDataBlob.h"
#include <QtCore/QDataStream>
#include <iostream>

namespace pelican {

void TestDataBlob::serialise(QIODevice& device) const
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << _data;
    device.write(array);
}

void TestDataBlob::deserialise(QIODevice& device, QSysInfo::Endian)
{
    QDataStream in(&device);
    in.setVersion(QDataStream::Qt_4_0);
    in >> _data;
}

} // namespace pelican

