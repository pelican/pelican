#include "TestDataBlob.h"


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

} // namespace pelican
