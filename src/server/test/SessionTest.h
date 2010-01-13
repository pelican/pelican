#ifndef SESSIONTEST_H
#define SESSIONTEST_H

#include <QByteArray>
#include <QString>
class QCoreApplication;
//class QDataStream;
#include <cppunit/extensions/HelperMacros.h>

/**
 * @file SessionTest.h
 */

namespace pelican {

class TestProtocol;
class Session;
class DataManager;
class DataBuffer;

/**
 * @class SessionTest
 *  
 * @brief
 *   Unit Test for the Session class
 * @details
 * 
 */

class SessionTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( SessionTest );
        CPPUNIT_TEST( test_processServiceDataRequest );
        CPPUNIT_TEST( test_processRequest );
        CPPUNIT_TEST( test_dataReport );
        CPPUNIT_TEST( test_serviceData );
        CPPUNIT_TEST( test_streamData );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_processRequest();
        void test_streamData();
        void test_processServiceDataRequest();
        void test_serviceData();
        void test_dataReport();

    public:
        SessionTest();
        ~SessionTest();

    protected:
        QString _injectData(DataBuffer* stream, const QString id = QString("") );

    private:
        QCoreApplication* _app;
        QDataStream* _out;
        Session* _session;
        QByteArray _block;
        TestProtocol* _proto;
        DataManager* _data;
};

} // namespace pelican
#endif // SESSIONTEST_H 
