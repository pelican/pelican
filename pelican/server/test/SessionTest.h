#ifndef SESSIONTEST_H
#define SESSIONTEST_H

/**
 * @file SessionTest.h
 */

#include <cppunit/extensions/HelperMacros.h>

#include <QtCore/QByteArray>
#include <QtCore/QString>

class QCoreApplication;
class QBuffer;

namespace pelican {

class Session;
class DataManager;
class AbstractDataBuffer;

namespace test {
class TestProtocol;
}

/**
 * @ingroup t_server
 *
 * @class SessionTest
 *
 * @brief
 * Unit Test for the Session class
 *
 * @details
 */

class SessionTest : public CppUnit::TestFixture
{
    public:
        CPPUNIT_TEST_SUITE( SessionTest );
        CPPUNIT_TEST( test_processServiceDataRequest );
        CPPUNIT_TEST( test_dataReport );
        CPPUNIT_TEST( test_serviceData );
        CPPUNIT_TEST( test_streamData );
        CPPUNIT_TEST( test_streamDataBufferFull );
        CPPUNIT_TEST( test_processRequest );
        CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

        // Test Methods
        void test_processRequest();
        void test_streamData();
        void test_streamDataBufferFull();
        void test_processServiceDataRequest();
        void test_serviceData();
        void test_dataReport();

    public:
        SessionTest();
        ~SessionTest();

    private:
        QString _injectData(AbstractDataBuffer* stream,
                const QString& id = QString(), int size = 10);

    private:
        QCoreApplication* _app;
        QDataStream* _out;
        Session* _session;
        QByteArray* _block;
        QBuffer* _device;
        test::TestProtocol* _proto;
        DataManager* _dataManager;
};

} // namespace pelican
#endif // SESSIONTEST_H
