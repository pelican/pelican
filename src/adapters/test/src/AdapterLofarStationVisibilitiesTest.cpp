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
    QDomElement e;
    ConfigNode config(e);
    AbstractStreamAdapter* adapter = new AdapterLofarStationVisibilities(config);
    unsigned nAnt = 2;
    unsigned nChan = 2;
    unsigned nPol = 2;
    VisGen g;
    g.generate(nAnt, nChan, nPol);
    g.print();
    g.write(fileName.toStdString());
    QByteArray b(g.dataStream());
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(g.size()), (int)b.size());

    QBuffer buffer(&b);
    buffer.open(QBuffer::ReadWrite);

    DataBlob *data = new VisibilityData(nAnt, nChan, nPol);
    QHash<QString, DataBlob*> h;
    adapter->config(data, g.size(), h);
    try {
        adapter->deserialise(&buffer);
    }
    catch (QString err) {
        std::cout << err.toStdString() << std::endl;
    }

    QFile testFile(fileName);
    testFile.open(QIODevice::ReadOnly);
    adapter->config(data, g.size(), h);
    try {
        adapter->deserialise(&testFile);
    }
    catch (QString err) {
        std::cout << err.toStdString() << std::endl;
    }

    delete adapter;
    delete data;
}

} // namespace pelican
