#include "ConfigTest.h"
#include "Config.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <iostream>

#include "utility/memCheck.h"

#define HERE std::cout << "** " <<__FUNCTION__ << "()::L" << __LINE__ << std::endl;

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
    // Use case
    // Constructs a configuration file with an invalid doc type
    // Expect to throw an exception
}


/**
 * @details
 * Test for setting configuration options
 */
void ConfigTest::test_setConfiguration()
{
    Config config;

    // Use case
    // Sets a configuration.
    // Expect that it exists.
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("modules", "");
        address << Config::NodeId_t("module", "test1");
        QDomElement e = config.set(address);
        CPPUNIT_ASSERT(e.parentNode().toElement().tagName() == "modules");
        CPPUNIT_ASSERT(e.tagName() == "module");
        CPPUNIT_ASSERT(e.attribute("name") == "test1");
    }

    /// Use case
    /// Sets a configuration by address and attribute
    /// Expects that it exists
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("servers", "");
        address << Config::NodeId_t("server", "test");
        QString ip = "123.456.789.10";
        config.setAttribute(address, "ip", ip);
        CPPUNIT_ASSERT(config.getAttribute(address, "ip") == ip);
    }
}


/**
 * @details
 * Test for setting configuration options
 */
void ConfigTest::test_getConfiguration()
{
    Config config;

    // Use case
    // Ask for an address that doesn't exist.
    // Expect that a null element is returned.
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("wibble", "wobble");
        QDomElement e = config.get(address);
        CPPUNIT_ASSERT(e.isNull());
    }

    // Use case
    // Ask for an attribute that doesn't exist (both address and key).
    // Expect that an empty string is returned
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("wibble", "wobble");
        QString a = config.getAttribute(address, "flop");
        CPPUNIT_ASSERT(a.isEmpty());
    }

    // Use case
    // Ask for an attribute with a key that doesn't exist at a valid address.
    // Expect that an empty string is returned
    {
        Config::TreeAddress_t address;
        address << Config::NodeId_t("wibble", "wobble");
        config.setAttribute(address, "flip", "true");
        QString a = config.getAttribute(address, "flop");
        CPPUNIT_ASSERT(a.isEmpty());
    }

}


/**
 * @details
 * Test for reading a configuration xml from file.
 */
void ConfigTest::test_configFileRead()
{
    QDir dir("utility/test/data");
    if (!dir.exists()) dir = QDir("test/data");
    else if (!dir.exists()) dir = QDir("data");
    else if (!dir.exists()) return;

    // Use case
    // Ask for the address of a module node in the test config file and check
    // that its children and their attributes are correct.
    // Expect that all of the parameters are found
    {
        QString fileName = dir.path() + "/testConfig.xml";
        QFile file(fileName);
        if (!file.exists()) return;
        Config config(fileName);
        Config::TreeAddress_t address;
        address << Config::NodeId_t("modules", "");
        address << Config::NodeId_t("module", "testA");
        QDomElement e = config.get(address);

        CPPUNIT_ASSERT(e.parentNode().nodeName() == "modules");
        CPPUNIT_ASSERT(e.childNodes().count() == 3);
        CPPUNIT_ASSERT(e.childNodes().at(0).nodeName() == "paramA");
        CPPUNIT_ASSERT(e.childNodes().at(1).nodeName() == "paramB");
        CPPUNIT_ASSERT(e.childNodes().at(0).toElement().attribute("name") == "some_value");
        CPPUNIT_ASSERT(e.childNodes().at(0).toElement().attribute("value") == "1");
        CPPUNIT_ASSERT(e.childNodes().at(1).toElement().attribute("name") == "some_coords");
        CPPUNIT_ASSERT(e.childNodes().at(1).toElement().attribute("x") == "1024");
        CPPUNIT_ASSERT(e.childNodes().at(1).toElement().attribute("y") == "2048");
        address << Config::NodeId_t("paramC", "channels");
        CPPUNIT_ASSERT(config.getText(address) == "1,2,3,4,5");
    }

    // Use case
    // Ask for the address of another module to to check this exists too.
    // Expect that the module node is found but has no children.
    {
        QString fileName = dir.path() + "/testConfig.xml";
        Config config(fileName);
        Config::TreeAddress_t address;
        address << Config::NodeId_t("modules", "");
        address << Config::NodeId_t("module", "default::testA");
        QDomElement e = config.get(address);
        CPPUNIT_ASSERT(e.parentNode().nodeName() == "modules");
        CPPUNIT_ASSERT(e.nodeName() == "module");
        CPPUNIT_ASSERT(e.childNodes().count() == 0);
    }

    // Use case
    // Try to read an empty xml file
    // Expect that a parse error exception is thrown
    {
        try {
            QString fileName = dir.path() + "/emptyConfig.xml";
            Config config(fileName);
        }
        catch (QString err) {
            CPPUNIT_ASSERT(err.startsWith("Parse error"));
        }
    }

    // Use case
    // Try to read a xml file with a bad doctype
    // Expect to throw an exception.
    {
        try {
            QString fileName = dir.path() + "/badConfig.xml";
            Config config(fileName);
        }
        catch (QString err) {
            CPPUNIT_ASSERT(err == "Invalid doctype.");
        }
    }

    // Use case
    // Try to extract the named parameters.
    // Expect that all the parameters are found.
    {
        QString fileName = dir.path() + "/testConfig.xml";
        Config config(fileName);
        Config::TreeAddress_t address;
        address << Config::NodeId_t("modules", "");
        address << Config::NodeId_t("module", "testA");
        QDomElement e = config.get(address);

        CPPUNIT_ASSERT(e.parentNode().nodeName() == "modules");
        CPPUNIT_ASSERT(e.namedItem("paramA").toElement().attribute("value") == "1");
        CPPUNIT_ASSERT(e.namedItem("paramB").toElement().attribute("x") == "1024");
        CPPUNIT_ASSERT(e.namedItem("paramB").toElement().attribute("name") == "some_coords");
    }
}


} // namespace pelican
