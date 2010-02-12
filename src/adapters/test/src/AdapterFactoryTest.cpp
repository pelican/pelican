#include "AdapterFactoryTest.h"
#include "AdapterFactory.h"
#include "adapters/AbstractAdapter.h"
#include "utility/Config.h"

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( AdapterFactoryTest );

/**
 * @details
 * AdapterFactoryTest constructor.
 */
AdapterFactoryTest::AdapterFactoryTest()
    : CppUnit::TestFixture()
{
}

/**
 * @details
 * AdapterFactoryTest destructor.
 */
AdapterFactoryTest::~AdapterFactoryTest()
{
}

/**
 * @details
 * Set-up routine called before each test.
 */
void AdapterFactoryTest::setUp()
{
}

/**
 * @details
 * Clean-up routine called after each test.
 */
void AdapterFactoryTest::tearDown()
{
}

/**
 * @details
 */
void AdapterFactoryTest::test_create()
{
}

} // namespace pelican
