#include "AdapterLofarStationVisibilitiesTest.h"
#include "adapters/test/VisGen.h"
#include <QCoreApplication>
#include <QDomElement>
#include <QDataStream>
#include <QBuffer>
#include <QFile>
#include "utility/ConfigNode.h"
#include "adapters/AdapterLofarStationVisibilities.h"
#include "adapters/AbstractStreamAdapter.h"
#include <iostream>
#include <iomanip>
#include "utility/pelicanTimer.h"


#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AdapterLofarStationVisibilitiesTest );
// class FileAdapterLofarVisibilitiesTest
AdapterLofarStationVisibilitiesTest::AdapterLofarStationVisibilitiesTest()
    : CppUnit::TestFixture()
{
}

AdapterLofarStationVisibilitiesTest::~AdapterLofarStationVisibilitiesTest()
{
}

void AdapterLofarStationVisibilitiesTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);
}

void AdapterLofarStationVisibilitiesTest::tearDown()
{
    delete _app;
}

void AdapterLofarStationVisibilitiesTest::test_method()
{
    QString fileName = "hello.dat";

    /* Create visibility data blob */
    const unsigned nAnt = 96;
    const unsigned nChan = 512;
    const unsigned nPol = 2;
    DataBlob *data = new VisibilityData(nAnt, nChan, nPol);

    /* Create the adapter */
    QDomElement e;
    ConfigNode config(e);
    AbstractStreamAdapter* adapter = new AdapterLofarStationVisibilities(config);

    /* Generate and write data to file */
    VisGen generator;
    generator.generate(nAnt, nChan, nPol);
    generator.write(fileName.toStdString());

    /* Create a buffer from the generated data and deserialise it */
    QByteArray array(generator.byteArray());
    CPPUNIT_ASSERT_EQUAL((int)generator.size(), array.size());
    QBuffer buffer(&array);
    buffer.open(QBuffer::ReadWrite);
    adapter->config(data, generator.size(), QHash<QString, DataBlob*>());
    try {
        TIMER_START
        adapter->deserialise(&buffer);
        TIMER_STOP("Time to deserialise %.2f MByte buffer",
                (float)buffer.size()/(1024*1024))
    }
    catch (QString err) {
        std::cout << err.toStdString() << std::endl;
    }

    /* Read the data file from the generated data and deserialise it */
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    adapter->config(data, generator.size(), QHash<QString, DataBlob*>());
    try {
        TIMER_START
        adapter->deserialise(&file);
        TIMER_STOP("Time to deserialise %.2f MByte file",
                (float)file.size()/(1024*1024))
    }
    catch (QString err) {
        std::cout << err.toStdString() << std::endl;
    }

    delete adapter;
    delete data;
}

} // namespace pelican
