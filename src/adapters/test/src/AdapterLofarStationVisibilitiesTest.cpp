#include "AdapterLofarStationVisibilitiesTest.h"
#include "adapters/test/VisGen.h"
#include <QCoreApplication>
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

void AdapterLofarStationVisibilitiesTest::test_deserialise_buffer()
{
    /* Create visibility data blob */
    const unsigned nAnt = 96;
    const unsigned nChan = 512;
    const unsigned nPol = 2;
    VisibilityData *data = new VisibilityData(nAnt, nChan, nPol);

    /* Create the adapter */
    ConfigNode config;
    AbstractStreamAdapter* adapter = new AdapterLofarStationVisibilities(config);

    /* Generate data */
    VisGen generator;
    generator.generate(nAnt, nChan, nPol);

    /* Create a buffer from the generated data */
    QByteArray array = generator.byteArray();
    QBuffer buffer(&array);
    buffer.open(QBuffer::ReadOnly);
    CPPUNIT_ASSERT_EQUAL((int)generator.size(), (int)buffer.size());
    adapter->config(data, generator.size(), QHash<QString, DataBlob*>());

    /* Deserialise the data */
    TIMER_START
    CPPUNIT_ASSERT_NO_THROW(adapter->deserialise(&buffer));
    TIMER_STOP("Time to deserialise %.2f MByte buffer",
            (float)buffer.size()/(1024*1024))

    /* Test some values in the visibility data blob */
    unsigned ai = 0, aj = 0, c = 0, p = 0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    ai = 1; aj = 2; c = 3; p = 1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    ai = 12; aj = 34; c = 56; p = 0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    ai = nAnt-1; aj = nAnt-2; c = nChan-1; p = nPol-1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    /* Clean up */
    delete adapter;
    delete data;
}

void AdapterLofarStationVisibilitiesTest::test_deserialise_file()
{
    /* Create visibility data blob */
    const unsigned nAnt = 96;
    const unsigned nChan = 512;
    const unsigned nPol = 2;
    VisibilityData *data = new VisibilityData(nAnt, nChan, nPol);

    /* Create the adapter */
    ConfigNode config;
    AbstractStreamAdapter* adapter = new AdapterLofarStationVisibilities(config);

    /* Generate data */
    VisGen generator;
    generator.generate(nAnt, nChan, nPol);

    /* Create a file from the generated data */
    QString fileName = "deserialise_file.dat";
    generator.write(fileName.toStdString());
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    adapter->config(data, generator.size(), QHash<QString, DataBlob*>());

    /* Deserialise the data */
    TIMER_START
    CPPUNIT_ASSERT_NO_THROW(adapter->deserialise(&file));
    TIMER_STOP("Time to deserialise %.2f MByte file",
            (float)file.size()/(1024*1024))

    /* Test some values in the visibility data blob */
    unsigned ai = 0, aj = 0, c = 0, p = 0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    ai = 1; aj = 2; c = 3; p = 1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    ai = 12; aj = 34; c = 56; p = 0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    ai = nAnt-1; aj = nAnt-2; c = nChan-1; p = nPol-1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(_dataVal(ai, aj, c, p),
            (*data)(ai, aj, c, p).real(), 0.001);

    /* Clean up */
    delete adapter;
    delete data;
    file.remove();
}

} // namespace pelican
