#include <QtCore/QVector>
#include <QtCore/QString>
#include "DataBlobBufferTest.h"
#include "DataBlobBuffer.h"
#include "TestDataBlob.h"


namespace pelican {
using namespace test;

CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobBufferTest );

/**
 *@details DataBlobBufferTest 
 */
DataBlobBufferTest::DataBlobBufferTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataBlobBufferTest::~DataBlobBufferTest()
{
}

void DataBlobBufferTest::setUp()
{
}

void DataBlobBufferTest::tearDown()
{
}

void DataBlobBufferTest::test_nextMethod()
{
     { // Use Case:
       // history set to n > 1
       // Expect:
       // next to return a different buffer for each call
       // until it reaches n, then recycle them in the same order
       for(int hmax=1; hmax<3; ++hmax ) {
           QVector<TestDataBlob* > blobs; // keep a track of blob order
           DataBlobBuffer buffer;
           // set up the blob buffer
           for(int history=0; history<hmax; ++history ) {
               TestDataBlob* blob=new TestDataBlob;
               buffer.addDataBlob(blob);
               blobs.append(blob);
           }
           // test the next method
           for(int cycles=0; cycles<2; ++cycles ) {
               for(int history=0; history<hmax; ++history ) {
                   TestDataBlob* b = static_cast<TestDataBlob*>(buffer.next());
                   QString msg = QString("history size=%1 cycle %2, blobnumber=%3, blob expected=%4, got blob %5").arg(hmax).arg(cycles).arg(history).arg((long)blobs[history]).arg((long)b);
                   CPPUNIT_ASSERT_MESSAGE(  msg.toStdString(), b == blobs[history] );
               }
           }
        }
     }
}

} // namespace pelican
