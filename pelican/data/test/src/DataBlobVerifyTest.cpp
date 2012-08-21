#include "DataBlobVerifyTest.h"
#include "DataBlobVerify.h"
#include "pelican/data/test/TestDataBlob.h"
#include <QtCore/QDebug>

namespace pelican {
namespace test {
class BrokenTestDataBlob: public test::TestDataBlob {
    public:
        BrokenTestDataBlob() : TestDataBlob("BrokenTestDataBlob") {}
        ~BrokenTestDataBlob() {}
        virtual quint64 serialisedBytes() const { 
            return 2*TestDataBlob::serialisedBytes(); }
        virtual void deserialise(QIODevice&, QSysInfo::Endian ) {
            data()="some rubbish";
        };
};
PELICAN_DECLARE_DATABLOB(BrokenTestDataBlob);
} // namespace test


CPPUNIT_TEST_SUITE_REGISTRATION( DataBlobVerifyTest );
/**
 *@details DataBlobVerifyTest 
 */
DataBlobVerifyTest::DataBlobVerifyTest()
    : CppUnit::TestFixture()
{
}

/**
 *@details
 */
DataBlobVerifyTest::~DataBlobVerifyTest()
{
}

void DataBlobVerifyTest::setUp()
{
    _testBlob = new test::TestDataBlob;
    _testBlob->setData("abc");
}

void DataBlobVerifyTest::tearDown()
{
    delete _testBlob;
}

void DataBlobVerifyTest::test_verifySerialisedBytes()
{
    DataBlobVerify dbv( _testBlob );
    CPPUNIT_ASSERT(dbv.verifySerialisedBytes());

    test::BrokenTestDataBlob badBlob;
    badBlob.setData("abc");
    DataBlobVerify vb( &badBlob );
    CPPUNIT_ASSERT( ! vb.verifySerialisedBytes());
}

void DataBlobVerifyTest::test_verifyDeSerialise()
{
    DataBlobVerify dbv( _testBlob );
    CPPUNIT_ASSERT(dbv.verifyDeserialise());

    test::BrokenTestDataBlob badBlob;
    badBlob.setData("abc");
    DataBlobVerify vb( &badBlob );
    CPPUNIT_ASSERT( ! vb.verifyDeserialise());
}

} // namespace pelican
