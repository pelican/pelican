#include "BasicFlaggerTest.h"
#include "modules/BasicFlagger.h"
#include "data/VisibilityData.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( BasicFlaggerTest );
// class BasicFlaggerTest 
BasicFlaggerTest::BasicFlaggerTest()
    : CppUnit::TestFixture()
{
}

BasicFlaggerTest::~BasicFlaggerTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void BasicFlaggerTest::setUp()
{
    /* Create a new module with empty configuration before each test */
    _basicFlagger = new BasicFlagger(QDomElement());
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void BasicFlaggerTest::tearDown()
{
    /* Delete the module after each test */
    delete _basicFlagger;
}

/**
 * @details
 * Tests the run() method of the BasicFlagger using an empty data hash,
 * and a hash containing no visibilities.
 */
void BasicFlaggerTest::test_run_noData()
{
    // Use Case
    // Pass an empty data blob hash.
    // Expect an exception.
    {
        QHash<QString, DataBlob*> emptyData;
        CPPUNIT_ASSERT_THROW(_basicFlagger->run(emptyData), QString);
    }

    // Use Case
    // Pass a data blob hash containing no visibility data.
    // Expect an exception.
    {
        QHash<QString, DataBlob*> data;
        DataBlob blob;
        data.insert("EmptyBlob", &blob);
        CPPUNIT_ASSERT_THROW(_basicFlagger->run(data), QString);
    }
}

/**
 * @details
 * Tests the run() method of the BasicFlagger using supplied
 * visibility data.
 */
void BasicFlaggerTest::test_run_withData()
{
    // Use Case
    // Create a set of test visibility data and pass it to the module.
    unsigned nAntennas = 96;
    unsigned nChannels = 512;
    unsigned nPols = 2;
    unsigned nTotal = nAntennas * nAntennas * nChannels * nPols;
    VisibilityData visData(nAntennas, nChannels, nPols);



    QHash<QString, DataBlob*> data;
    data.insert("VisibilityData", &visData);

    CPPUNIT_ASSERT_NO_THROW(_basicFlagger->run(data));
}

} // namespace pelican
