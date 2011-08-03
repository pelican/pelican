#include "pelican/utility/test/ConfigNodeTest.h"
#include "pelican/utility/ConfigNode.h"

#include <QtCore/QCoreApplication>

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


/**
 * @details
 */
void ConfigNodeTest::test_setFromString()
{
    QString xml = ""
            "<node>"
                "<property1 attribute=\"value1\"/>"
                "<property2 attribute=\"value2\"/>"
            "</node>";
    ConfigNode node;
    node.setFromString(xml);
    CPPUNIT_ASSERT(node.getOption("property1", "attribute", "") == QString("value1"));
    CPPUNIT_ASSERT(node.getOption("property2", "attribute", "") == QString("value2"));
}




} // namespace pelican
