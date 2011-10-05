#include <QtCore/QVector>
#include <QtCore/QString>
#include "DataBlobBufferTest.h"
#include "DataBlobBuffer.h"
#include "TestDataBlob.h"
#include "DataBlob.h"


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

void DataBlobBufferTest::test_shrink()
{
       QVector<TestDataBlob* > blobs; // keep a track of blob order
       DataBlobBuffer buffer;
       for(int history=0; history<10; ++history ) {
           TestDataBlob* blob=new TestDataBlob;
           buffer.addDataBlob(blob);
           blobs.append(blob);
        }
        { // Use Case:
          // Shrink To value >= than existing size
          // Expect:
          // nothing to change
          buffer.shrink(11);
          CPPUNIT_ASSERT_EQUAL( (unsigned int)10, buffer.size() );
          buffer.shrink(10);
          CPPUNIT_ASSERT_EQUAL( (unsigned int)10, buffer.size() );
        }
        { // Use Case:
          // Shrink To value < existing size
          // next has not yet been called
          // Expect:
          // buffer to be set to the size specified
          // next to return a valid object
          buffer.shrink(9);
          CPPUNIT_ASSERT_EQUAL( (unsigned int)9, buffer.size() );
          DataBlob* b = buffer.next();
          int history=1;
          QString msg = QString("blobnumber=%3, blob expected=%4, got blob %5").arg(history).arg((long)blobs[history],0,16).arg((long)b,0,16);
          CPPUNIT_ASSERT_MESSAGE(  msg.toStdString(), b == blobs[history] );

          // ensure index is still keeping track
          history = 4; // the next we expect from a call to next (2,3 shold be deleted)
          buffer.shrink(7);
          b = buffer.next();
          msg = QString("blobnumber=%3, blob expected=%4, got blob %5").arg(history).arg((long)blobs[history],0,16).arg((long)b,0,16);
          CPPUNIT_ASSERT_MESSAGE(  msg.toStdString(), b == blobs[history] );
        }
}

void DataBlobBufferTest::dump(const QVector<TestDataBlob* >& blobs)
{
        for( int i=0; i < blobs.size(); ++i ) {
            std::cout << i << " = " << blobs[i] << std::endl;
        }
}

} // namespace pelican
