#ifndef TESTPIPELINESERVERTEST_H
#define TESTPIPELINESERVERTEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <QThread>

/**
 * @file TestPipelineServerTest.h
 */

namespace pelican {

class ConfigNode;
class TestConfig;

/**
 * @class TestPipelineServerTest
 *
 * @brief
 *
 * @details
 *
 */
class TestPipelineServerTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( TestPipelineServerTest );
        CPPUNIT_TEST( test_testChunker );
        CPPUNIT_TEST( test_testUdpChunker );
        CPPUNIT_TEST( test_testTwoUdpChunkers );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_testChunker();
        void test_testUdpChunker();
        void test_testTwoUdpChunkers();

    private:
        // Create XML configuration
        void _createConfig();

    public:
        TestPipelineServerTest();
        ~TestPipelineServerTest();

    private:
        ConfigNode* _emulatorConfig1;
        ConfigNode* _emulatorConfig2;
};


class PipelineBinaryEmulator : public QThread
{
    private:
        TestConfig* _config;

    public:
        PipelineBinaryEmulator(TestConfig *config) : QThread(),
        _config(config) {start();}

    protected:
        void run();
};

} // namespace pelican

#endif // TESTPIPELINESERVERTEST_H
