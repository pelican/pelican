#include "SessionTest.h"
#include <QBuffer>
#include <QByteArray>
#include <QDataStream>
#include <QCoreApplication>
#include "Session.h"
#include "DataManager.h"
#include "LockableStreamData.h"
#include "WritableData.h"
#include "StreamDataBuffer.h"
#include "data/DataRequirements.h"
#include "comms/Data.h"
#include "comms/StreamData.h"
#include "comms/ServerRequest.h"
#include "comms/ServiceDataRequest.h"
#include "comms/StreamDataRequest.h"
#include "TestProtocol.h"
#include "utility/pelicanTimer.h"
#include <QTime>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( SessionTest );
// class SessionTest
SessionTest::SessionTest()
    : CppUnit::TestFixture()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

SessionTest::~SessionTest()
{
    delete _app;
}

void SessionTest::setUp()
{
    QString id = "1";
    _proto = new TestProtocol(id);
    _dataManager = new DataManager;
    _session = new Session(0, _proto, _dataManager);
    _block = new QByteArray;
    _device = new QBuffer(_block);
}

void SessionTest::tearDown()
{
    delete _proto;
    delete _dataManager;
    delete _session;
    delete _block;
    delete _device;
}

void SessionTest::test_processRequest()
{
    {
        // Use Case:
        // Request an acknowledgement
        // Expect the id to be returned
        ServerRequest request(ServerRequest::Acknowledge);

        CPPUNIT_ASSERT_EQUAL( 0, _block->size() );
        _session->processRequest(request, *_device);
        CPPUNIT_ASSERT( "ACK" ==  _proto->lastBlock() );
    }
    // Set up stream data for remaining tests
    QString stream1("test");
    StreamDataBuffer* streambuffer = new StreamDataBuffer(stream1);
    _dataManager->setStreamDataBuffer( stream1, streambuffer );
    {
        // Use Case:
        // Request a single StreamData, no data available
        // Expect send StreamData on protocol not to be called
        DataRequirements req;
        req.setStreamData(stream1);
        StreamDataRequest request;
        request.addDataOption(req);
        CPPUNIT_ASSERT_EQUAL( 0, _block->size() );
        _session->processRequest(request, *_device);
        AbstractProtocol::StreamData_t data = _proto->lastStreamData();
        CPPUNIT_ASSERT_EQUAL( 0, data.size() );
    }
    _injectData(streambuffer, "version1");
    {
        // Use Case:
        // Request a single StreamData, with data available
        // Expect send StreamData on protocol to be called
        DataRequirements req;
        req.setStreamData(stream1);
        StreamDataRequest request;
        request.addDataOption(req);
        CPPUNIT_ASSERT_EQUAL( 0, _block->size() );
        _session->processRequest(request, *_device);
        AbstractProtocol::StreamData_t data = _proto->lastStreamData();
        CPPUNIT_ASSERT_EQUAL( 1, data.size() );
        CPPUNIT_ASSERT_EQUAL( stream1.toStdString(), data[0]->name().toStdString() );
        CPPUNIT_ASSERT_EQUAL( 10 , (int)data[0]->size() );
        CPPUNIT_ASSERT_EQUAL( 0 , (int)data[0]->associateData().size() );
    }
}

void SessionTest::test_dataReport()
{
}

void SessionTest::test_serviceData()
{
    {
        // Use Case:
        // Request ServiceData that is not supported
        // Expect error message to be returned
        QDataStream out(_block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

        ServiceDataRequest request;
        _session->processRequest(request, *_device);
    }
}

void SessionTest::test_processServiceDataRequest()
{
    {
        // Use Case:
        // ServiceDataRequest is empty
        // expect empty list
        ServiceDataRequest request;
        QList<LockedData> dataList = _session->processServiceDataRequest(request);
        CPPUNIT_ASSERT( dataList.size() == 0  );
    }
    QString badversion("badv");
    QString badtype("bad");
    QString type1("type1");
    ServiceDataBuffer* servicebuffer1 = new ServiceDataBuffer("test");
    _dataManager->setServiceDataBuffer( type1, servicebuffer1 );
    QString version1 = _injectData(servicebuffer1, "version1");
    QString type2("type2");
    ServiceDataBuffer* servicebuffer2 = new ServiceDataBuffer("test");
    _dataManager->setServiceDataBuffer( type2, servicebuffer2 );
    QString version2 = _injectData(servicebuffer2, "version2");
    {
        // Use Case:
        // ServiceDataRequest for data that does not exist
        // expect throw
        ServiceDataRequest request;
        request.request(type1, badversion);
        CPPUNIT_ASSERT_THROW( _session->processServiceDataRequest(request), QString );
    }
    {
        // Use Case:
        // ServiceDataRequest for two objects , one not existing
        // expect throw
        ServiceDataRequest request;
        request.request(type1, version1);
        request.request(badtype, version1);
        CPPUNIT_ASSERT_THROW( _session->processServiceDataRequest(request), QString );
    }
    {
        // Use Case:
        // ServiceDataRequest for two objects , both existing
        // expect a list of objects
        ServiceDataRequest request;
        request.request(type1, version1);
        request.request(type2, version2);
        QList<LockedData> dataList = _session->processServiceDataRequest(request);
        CPPUNIT_ASSERT_EQUAL( 2, dataList.size() );
    }
}

void SessionTest::test_streamData()
{
    {
        // Use Case:
        // Request StreamData that has no data request options (empty request).
        // Expect request to be silently ignored and an empty list to be returned.
        StreamDataRequest request;
        QList<LockedData> dataList = _session->processStreamDataRequest(request);
        CPPUNIT_ASSERT_EQUAL(0, dataList.size());
    }

    // Set up stream data for remaining tests.
    QString stream1("stream1");
    StreamDataBuffer* streambuffer = new StreamDataBuffer(stream1);
    _dataManager->setStreamDataBuffer( stream1, streambuffer );

    {
        // Use Case:
        // Request StreamData for a stream that is not supported by the server.
        // Expect error message to be returned.
        DataRequirements requirements;   
        requirements.setStreamData("NotSupported");
        StreamDataRequest request;
        request.addDataOption(requirements);
        try {
            _session->processStreamDataRequest(request);
        }
        catch (QString error) {
            CPPUNIT_ASSERT(error.contains("Data requested not supported by server"));
        }
    }

    {
        // Use Case:
        // Request StreamData for a stream that is supported but for which no
        // data exists and set the request timeout.
        // Expect to throw an error after the timeout as been exceeded.
        DataRequirements requirements;
        requirements.setStreamData(stream1);
        StreamDataRequest request;
        request.addDataOption(requirements);
        try {
            _session->processStreamDataRequest(request, 100);
        }
        catch (QString error) {
            CPPUNIT_ASSERT(error.contains("Request timed out"));
        }
    }

    {
        // Use Case:
        // Request StreamData for a stream that is supported and the data exists
        // no service data.
        // Expect to return the data.
        DataRequirements requirements;
        requirements.setStreamData(stream1);
        
        StreamDataRequest request;
        request.addDataOption(requirements);
        _injectData(streambuffer);

        QList<LockedData> dataList = _session->processStreamDataRequest(request);
        foreach(LockedData data, dataList) {
            CPPUNIT_ASSERT( data.isValid() );
            CPPUNIT_ASSERT_EQUAL( 0 , (int)static_cast<LockableStreamData*>(data.object())->associateData().size() );
            CPPUNIT_ASSERT_EQUAL( 0 , (int)(static_cast<LockableStreamData*>(data.object())->streamData()->associateData().size()) );
        }
    }

    // Set up service data stream for remaining tests
    QString service1("service1");
    ServiceDataBuffer* servicebuffer = new ServiceDataBuffer("test");
    _dataManager->setServiceDataBuffer( service1, servicebuffer );

    {
        // Use Case:
        // Request StreamData for a stream that is supported and the data exists
        // service data requested, but not available
        // Expect to throw after the timeout has been reached and the stream
        // data to still be availaible in the buffer at the end.
        DataRequirements requirements;
        requirements.setStreamData(stream1);
        requirements.setServiceData(service1);
        StreamDataRequest request;
        request.addDataOption(requirements);
        //_injectData(streambuffer); // NOTE Dont need to inject data again.


        try {
            _session->processStreamDataRequest(request, 10);
        }
        catch (QString error) {
            CPPUNIT_ASSERT(error.contains("Request timed out"));
        }
        CPPUNIT_ASSERT(_dataManager->getNext(stream1).isValid() == true);
    }

    {
        DataManager myDataManager;
        ServiceDataBuffer* myServiceBuffer = myDataManager.getServiceBuffer(service1);
        StreamDataBuffer* myStreamBuffer = myDataManager.getStreamBuffer(stream1);
        Session mySession(0, _proto, &myDataManager);
        {
            // Use Case:
            // Request StreamData for a stream that is supported and the data exists
            // service data requested, and it exists
            // Expect to return data, and service data record

            std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;

            DataRequirements requirements;
            requirements.setStreamData(stream1);
            requirements.setServiceData(service1);
            StreamDataRequest request;
            request.addDataOption(requirements);

            QString version("0");
            try {
                _injectData(myServiceBuffer, version);
                _injectData(myStreamBuffer); // NOTE dont need to inject data again
            }
            catch (QString error) {
                std::cout << "ALERT !! " << error.toStdString() << std::endl;
            }

            // Check that the data buffers have data in them.
            CPPUNIT_ASSERT(myDataManager.getNext(stream1).isValid() == true);
            CPPUNIT_ASSERT(myDataManager.getServiceData(service1, version).isValid() == true);

            QList<LockedData> dataList;
            CPPUNIT_ASSERT_NO_THROW(dataList = mySession.processStreamDataRequest(request, 100));
            CPPUNIT_ASSERT_EQUAL(1, dataList.size());

            foreach(LockedData data, dataList) {
                CPPUNIT_ASSERT( data.isValid() );
                CPPUNIT_ASSERT_EQUAL( 1 , (int)static_cast<LockableStreamData*>(data.object())->associateData().size() );
                CPPUNIT_ASSERT_EQUAL( 1 , (int)(static_cast<LockableStreamData*>(data.object())->streamData()->associateData().size()) );
            }
        }
    }
}


QString SessionTest::_injectData(DataBuffer* buffer, const QString& id)
{
    // Writable data has to go out of scope to be activated.
    {
        WritableData d = buffer->getWritable(10);
        d.data()->setId(id);
    }
    _app->processEvents(); // Connect signals and slots to activate data.
    return id;
}

} // namespace pelican
