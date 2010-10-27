#include "pelican/data/test/TestDataBlob.h"

#include <QtCore/QDataStream>

namespace pelican {
namespace test {

void TestDataBlob::serialise(QIODevice& device) const
{
    QDataStream out(&device);
    out.setVersion(QDataStream::Qt_4_0);
    out << _data;
}

void TestDataBlob::deserialise(QIODevice& device, QSysInfo::Endian)
{
    QDataStream in(&device);
    in.setVersion(QDataStream::Qt_4_0);
    in >> _data;
}

} // namespace test
} // namespace pelican
