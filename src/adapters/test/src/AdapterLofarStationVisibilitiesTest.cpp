#include "AdapterLofarStationVisibilitiesTest.h"
#include <QDomElement>
#include <QDataStream>
#include "utility/ConfigNode.h"
#include "adapters/AdapterLofarStationVisibilities.h"
#include "adapters/AbstractStreamAdapter.h"
#include "core/test/VisGen.h"


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
}

void AdapterLofarStationVisibilitiesTest::tearDown()
{
}

void AdapterLofarStationVisibilitiesTest::test_method()
{
    QDomElement e;
    ConfigNode config(e);
    AbstractStreamAdapter* adapter = new AdapterLofarStationVisibilities(config);
    unsigned nAnt = 2;
    unsigned nChan = 2;
    unsigned nPol = 2;
    VisGen g;
    g.generate(nAnt, nChan, nPol);
    QDataStream *in = &g.dataStream();
    DataBlob *data = new VisibilityData(nAnt, nChan, nPol);
    QHash<QString, DataBlob*> h;
    *in >> adapter->config(data, 0, h);
    VisibilityData *vis = static_cast<VisibilityData*>(data);
    std::cout << vis->nEntries() << std::endl;
    delete adapter;
    delete data;
    delete in;
}

} // namespace pelican
