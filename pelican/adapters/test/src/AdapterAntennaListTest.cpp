#include "AdapterAntennaListTest.h"
#include "pelican/adapters/AdapterAntennaList.h"
#include "pelican/data/AntennaPositions.h"
#include "pelican/utility/ConfigNode.h"
#include <QCoreApplication>
#include <QDomElement>
#include <QFile>

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AdapterAntennaListTest );
/**
 *@details AdapterAntennaListTest
 */
AdapterAntennaListTest::AdapterAntennaListTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
AdapterAntennaListTest::~AdapterAntennaListTest()
{
}

void AdapterAntennaListTest::setUp()
{
    int argc = 1;
    char *argv[] = {(char*)"pelican"};
    _app = new QCoreApplication(argc, argv);
}

void AdapterAntennaListTest::tearDown()
{
    delete _app;
}

void AdapterAntennaListTest::test_method()
{
    QString fileName = "pelican/data/antennaPositions.dat";
    QFile file(fileName);
    if (!file.exists()) return;
    file.open(QIODevice::ReadOnly);
    AdapterAntennaList *adapter = new AdapterAntennaList(ConfigNode(QDomElement()));
    AntennaPositions data;
    adapter->config(&data, file.size());
    adapter->deserialise(&file);
    delete adapter;
}

} // namespace pelican
