#include "AdapterFactoryTest.h"
#include "AdapterFactory.h"
#include "pelican/adapters/AbstractAdapter.h"
#include "pelican/utility/Config.h"

#include "pelican/utility/memCheck.h"

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
