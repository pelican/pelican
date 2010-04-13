#include "ConfigNodeTest.h"
#include "ConfigNode.h"
#include <QCoreApplication>

#include "pelican/utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigNodeTest );
// class ConfigTest 
ConfigNodeTest::ConfigNodeTest()
    : CppUnit::TestFixture()
{
}

ConfigNodeTest::~ConfigNodeTest()
{
}

void ConfigNodeTest::setUp()
{
    int ac = 0;
    app = new QCoreApplication(ac, NULL);
}

void ConfigNodeTest::tearDown()
{
    delete app;
}

/**
 * @details
 * Test for empty configuration file name string.
 */
void ConfigNodeTest::test_methods()
{

}



} // namespace pelican
