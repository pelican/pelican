#include "AdapterRealDataTest.h"
#include "pelican/adapters/AdapterRealData.h"
#include "pelican/data/RealData.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/constants.h"

#include <QCoreApplication>
#include <QString>
#include <QBuffer>
#include <iostream>
#include <iomanip>

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AdapterRealDataTest );
// class AdapterRealDataTest
AdapterRealDataTest::AdapterRealDataTest()
    : CppUnit::TestFixture()
{
}

AdapterRealDataTest::~AdapterRealDataTest()
{
}

void AdapterRealDataTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);
}


void AdapterRealDataTest::tearDown()
{
    delete _app;
}


void AdapterRealDataTest::test_deserialise_buffer()
{
    // Create a buffer to deserialise.
    std::vector<real_t> array(10, 0);
    size_t chunkSize = array.size() * sizeof(real_t);
    for (unsigned i = 0; i < array.size(); ++i) array[i] = i;
    QByteArray chunk = QByteArray::fromRawData((char*)&array[0], chunkSize);
    QBuffer buffer(&chunk);
    buffer.open(QBuffer::ReadOnly);

    // Create the data blob and the adapter.
    RealData data;
    ConfigNode config;
    AdapterRealData adapter(config);
    adapter.config(&data, chunkSize, QHash<QString, DataBlob*>());

    // Deserialise the data.
    CPPUNIT_ASSERT_NO_THROW(adapter.deserialise(&buffer));

    // Test values in the data blob.
    for (unsigned i = 0; i < array.size(); ++i)
        CPPUNIT_ASSERT_DOUBLES_EQUAL(i, data.ptr()[i], 0.001);
}

} // namespace pelican
