#ifndef DIRECTSTREAMDATACLIENTTEST_H
#define DIRECTSTREAMDATACLIENTTEST_H

/**
 * @file DirectStreamDataClientTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

class QCoreApplication;

namespace pelican {

class Config;
class ConfigNode;
class DoubleData;

/**
 * @ingroup t_core
 *
 * @class DirectStreamDataClientTest
 *
 * @brief
 *
 * @details
 *
 */

class DirectStreamDataClientTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( DirectStreamDataClientTest );
        CPPUNIT_TEST( test_singleChunker );
        CPPUNIT_TEST( test_twoChunkersSingleStart );
        CPPUNIT_TEST( test_twoChunkersMultipleStarts );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_singleChunker();
        void test_twoChunkersMultipleStarts();
        void test_twoChunkersSingleStart();

    public:
        DirectStreamDataClientTest(  );
        ~DirectStreamDataClientTest();

    private:
        void _printData(DoubleData* data);

    private:
        Config* _config;
        ConfigNode* _emulatorConfig1;
        ConfigNode* _emulatorConfig2;
};

} // namespace pelican

#endif // DIRECTSTREAMDATACLIENTTEST_H
