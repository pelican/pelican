#include "pelican/utility/test/ConfigTest.h"
#include "pelican/utility/TestConfig.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <iostream>

namespace pelican {
using test::TestConfig;

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
        Config::TreeAddress address;
        address << Config::NodeId("modules", "");
        address << Config::NodeId("module", "test1");
        QDomElement e = config._set(address);
        CPPUNIT_ASSERT(e.parentNode().toElement().tagName() == "modules");
        CPPUNIT_ASSERT(e.tagName() == "module");
        CPPUNIT_ASSERT(e.attribute("name") == "test1");
    }

    /// Use case
    /// Sets a configuration by address and attribute
    /// Expects that it exists
    {
        Config::TreeAddress address;
        address << Config::NodeId("servers", "");
        address << Config::NodeId("server", "test");
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
        Config::TreeAddress address;
        address << Config::NodeId("wibble", "wobble");
        QDomElement e = config._get(address, config._document);
        CPPUNIT_ASSERT(e.isNull());
    }

    // Use case
    // Ask for an attribute that doesn't exist (both address and key).
    // Expect that an empty string is returned
    {
        Config::TreeAddress address;
        address << Config::NodeId("wibble", "wobble");
        QString a = config.getAttribute(address, "flop");
        CPPUNIT_ASSERT(a.isEmpty());
    }

    // Use case
    // Ask for an attribute with a key that doesn't exist at a valid address.
    // Expect that an empty string is returned
    {
        Config::TreeAddress address;
        address << Config::NodeId("wibble", "wobble");
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

    // Use case
    // Ask for the address of a module node in the test config file and check
    // that its children and their attributes are correct.
    // Expect that all of the parameters are found
    {
        TestConfig config("testConfig.xml", "utility");
        Config::TreeAddress address;
        address << Config::NodeId("modules", "");
        address << Config::NodeId("moduleType", "testA");
        QDomElement e = config._get(address, config._document);
        CPPUNIT_ASSERT(e.parentNode().nodeName() == "modules");
        CPPUNIT_ASSERT(e.childNodes().count() == 3);
        CPPUNIT_ASSERT(e.childNodes().at(0).nodeName() == "paramA");
        CPPUNIT_ASSERT(e.childNodes().at(1).nodeName() == "paramB");
        CPPUNIT_ASSERT(e.childNodes().at(0).toElement().attribute("name") == "some_value");
        CPPUNIT_ASSERT(e.childNodes().at(0).toElement().attribute("value") == "1");
        CPPUNIT_ASSERT(e.childNodes().at(1).toElement().attribute("name") == "some_coords");
        CPPUNIT_ASSERT(e.childNodes().at(1).toElement().attribute("x") == "1024");
        CPPUNIT_ASSERT(e.childNodes().at(1).toElement().attribute("y") == "2048");
        address << Config::NodeId("paramC", "channels");
        CPPUNIT_ASSERT(config.getText(address) == "1,2,3,4,5");
    }

    // Use case
    // Ask for the address of another module to to check this exists too.
    // Expect that the module node is found but has no children.
    {
        TestConfig config("testConfig.xml", "utility");
        Config::TreeAddress address;
        address << Config::NodeId("modules", "");
        address << Config::NodeId("moduleType", "default::testA");
        QDomElement e = config._get(address, config._document);
        CPPUNIT_ASSERT(e.parentNode().nodeName() == "modules");
        CPPUNIT_ASSERT(e.nodeName() == "moduleType");
        CPPUNIT_ASSERT(e.childNodes().count() == 0);
    }

    // Use case
    // Try to read an empty xml file
    // Expect that a parse error exception is thrown
    {
        try {
            TestConfig config("emptyConfig.xml", "utility");
        }
        catch (QString err) {
            CPPUNIT_ASSERT(err.startsWith("Config::read(): Parse error"));
        }
    }

    // Use case
    // Try to read a xml file with a bad doctype
    // Expect to throw an exception.
    {
        try {
            TestConfig config("badConfig.xml", "utility");
        }
        catch (QString err) {
            CPPUNIT_ASSERT(err.startsWith("Config::read(): Invalid doctype"));
        }
    }

    // Use case
    // Try to extract the named parameters.
    // Expect that all the parameters are found.
    {
        TestConfig config("testConfig.xml", "utility");
        Config::TreeAddress address;
        address << Config::NodeId("modules", "");
        address << Config::NodeId("moduleType", "testA");
        QDomElement e = config._get(address, config._document);

        CPPUNIT_ASSERT(e.parentNode().nodeName() == "modules");
        CPPUNIT_ASSERT(e.namedItem("paramA").toElement().attribute("value") == "1");
        CPPUNIT_ASSERT(e.namedItem("paramB").toElement().attribute("x") == "1024");
        CPPUNIT_ASSERT(e.namedItem("paramB").toElement().attribute("name") == "some_coords");
    }
}


/**
 * @details
 * Test for setting the configuration xml from a QString.
 */
void ConfigTest::test_configFromString()
{
    QString xml =
    "<modules>"
    "   <moduleType name=\"test\">"
    "       <param value=\"2.0\"/>"
    "   </moduleType>"
    "</modules>";

    Config config;
    config.setFromString(xml);

    Config::TreeAddress address;
    address << Config::NodeId("pipeline", "");
    address << Config::NodeId("modules", "");
    address << Config::NodeId("moduleType", "test");
    ConfigNode configNode = config.get(address);

    CPPUNIT_ASSERT(configNode.type() == "moduleType");
    CPPUNIT_ASSERT(configNode.name() == "test");
    CPPUNIT_ASSERT_EQUAL(2.0, configNode.getOption("param", "value").toDouble());
}


/**
 * @details
 * Test for the TestConfig test class.
 */
void ConfigTest::test_testConfig()
{
    // Use case:
    // Test configuration file that exists
    // Expect not to throw.
    {
#if QT_VERSION >= 0x040300
            CPPUNIT_ASSERT_NO_THROW(TestConfig("testConfig.xml", "utility"));
#else
            // below Qt 4.3 searching for a file in a directory is not supported
#endif
    }

    // Use case:
    // Test configuration file that doesn't exist.
    // Expect to throw with useful message.
    {
        CPPUNIT_ASSERT_THROW_MESSAGE(
                "TestConfig: Configuration file noConfig.xml not found.",
                TestConfig("noConfig.xml", "utility"),
                QString
        );
    }
}

/**
 * @details
 * Tests preprocessing the XML document.
 */
void ConfigTest::test_preprocess()
{
    QString pipeline =
            "<modules>"
            "    <Module1>"
            "        <parameter value=\"1\"/>"
            "        <import nodeset=\"set1\"/>"
            "    </Module1>"
            "    <Module2>"
            "        <import nodeset=\"set1\"/>"
            "        <parameter value=\"2\"/>"
            "    </Module2>"
            "</modules>"
            "<import nodeset=\"set2\"/>";
    QString server =
            "<import nodeset=\"set2\"/>";
//            "<import file=\"data/testConfig.xml\"/>";
    QString nodesets =
            "<nodeset name=\"set1\">"
            "    <common1 value=\"first common parameter\"/>"
            "    <common2 value=\"second common parameter\"/>"
            //"    <import nodeset=\"nested\"/>"
            "</nodeset>"
            "<nodeset name=\"set2\">"
            "    <group>"
            "        <Object1>"
            "            <tag1 value=\"tag1 from set2 for Object1.\"/>"
            "            <tag2 value=\"tag2 from set2 for Object1.\"/>"
            "        </Object1>"
            "        <Object2>"
            "            <tag1 value=\"tag1 from set2 for Object2.\"/>"
            "            <tag2 value=\"tag2 from set2 for Object2.\"/>"
            "        </Object2>"
            "    </group>"
            "</nodeset>"
            "<nodeset name=\"nested\">"
            "    <nested parameter=\"test\"/>"
            "</nodeset>";

    try {
        Config config;
        config.setFromString(pipeline, server, nodesets);
        //config.summary();
    }
    catch (QString e) {
        CPPUNIT_FAIL("Unexpected exception: " + e.toStdString());
    }
}

void ConfigTest::test_searchFile() 
{
     QString testDir = QCoreApplication::applicationDirPath();
     QString existing_full = QCoreApplication::applicationFilePath();
     QString existing_relative = QFileInfo( existing_full ).fileName();
     CPPUNIT_ASSERT_EQUAL( existing_full.toStdString(), 
                          (testDir + QDir::separator() + existing_relative).toStdString() ); // sanity check
     QString non_existing = "IDoNotExist.zero";
     QList<QString> paths;
     paths << testDir;
     {
     // Use case: no search path set, and full path given
     //           to an existing file
     // Expect:
     //           return the full path to the file
     Config config;
     QList<QString> paths;
     config.setSearchPaths( paths );
     CPPUNIT_ASSERT_EQUAL( existing_full.toStdString(), config.searchFile( existing_full ).toStdString() );
     }
     {
     // Use case: search path set, and full path given
     //           to an existing file
     // Expect:
     //           return the full path to the file
     Config config;
     config.setSearchPaths( paths );
     CPPUNIT_ASSERT_EQUAL( existing_full.toStdString(), config.searchFile( existing_full ).toStdString() );
     }
     {
     // Use case: no search path set, relative path given
     //           to an existing file
     // Expect:
     //           throw
     Config config;
     QList<QString> paths;
     config.setSearchPaths( paths );
     CPPUNIT_ASSERT_THROW( config.searchFile( existing_relative ).toStdString(), QString );
     }
     {
     // Use case: no search path set, relative path given
     //           to a non existing file
     // Expect:
     //           throw
     Config config;
     QList<QString> paths;
     config.setSearchPaths( paths );
     CPPUNIT_ASSERT_THROW( config.searchFile( non_existing ).toStdString(), QString );
     }
     {
     // Use case: search path set, relative path given
     //           to an existing file
     // Expect:
     //           return the full path to the file
     Config config;
     config.setSearchPaths( paths );
     config.searchFile( existing_relative );
     CPPUNIT_ASSERT_EQUAL( existing_full.toStdString(), 
                           config.searchFile( existing_relative ).toStdString() );
     }
}

} // namespace pelican
