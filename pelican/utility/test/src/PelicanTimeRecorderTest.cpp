#include "PelicanTimeRecorderTest.h"
#include "PelicanTimeRecorder.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( PelicanTimeRecorderTest );
/**
 * @details Constructs a PelicanTimeRecorderTest object.
 */
PelicanTimeRecorderTest::PelicanTimeRecorderTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details Destroys the PelicanTimeRecorderTest object.
 */
PelicanTimeRecorderTest::~PelicanTimeRecorderTest()
{
}

void PelicanTimeRecorderTest::setUp()
{
}

void PelicanTimeRecorderTest::tearDown()
{
}

void PelicanTimeRecorderTest::test_method()
{
    PelicanTimeRecorder recorder;
    recorder.start();
    sleep(1);
    recorder.tick("test");
    sleep(1);
    recorder.report();
}

} // namespace pelican
