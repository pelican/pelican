#include "ConfigTest.h"
#include "Config.h"
#include <QCoreApplication>
#include <iostream>
#include "utility/memCheck.h"


namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ConfigTest );
// class ConfigTest 
ConfigTest::ConfigTest()
    : CppUnit::TestFixture()
{
}

ConfigTest::~ConfigTest()
{
}

void ConfigTest::setUp()
{
    int ac = 0;
    app = new QCoreApplication(ac, NULL);
}

void ConfigTest::tearDown()
{
    delete app;
}

/**
 * @details
 * Test for empty configuration file name string.
 */
void ConfigTest::test_emptyFileName()
{
    // Use case
    // Constructs a configuration with a blank file name string.
    // Expected not to throw an exception.
    CPPUNIT_ASSERT_NO_THROW(Config config(""));
}


/**
 * @details
 * Test for configuration file that doesn't exist.
 */
void ConfigTest::test_fileDoesntExist()
{
    // Use case
    // Constructs a configuration file with a filename that doesn't exist
    // Expect to throw an exception
     CPPUNIT_ASSERT_THROW(Config config("wibble"), QString);
}


/**
 * @details
 * Test for invalid configuration file doc type.
 */
void ConfigTest::test_invalidDocType()
{
    /// Use case
    // Constructs a configuration file with an invalid doc type
    // Expect to throw an exception
}


/**
 * @details
 * Test for creating a module element within the DomDocument
 */
void ConfigTest::test_createModuleConfig()
{
    Config config("");
    QList<QPair<QString, QString> > address;
    address << QPair<QString, QString>("modules", "")
            << QPair<QString, QString>("module", "test");
    address.append(qMakePair(QString("param"), QString("a")));
    QDomElement e = config.setConfiguration(address);
}

} // namespace pelican
