#include "TestAdapterFactory.h"
#include "TestAdapter.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 *@details TestAdapterFactory 
 */
TestAdapterFactory::TestAdapterFactory(const pelican::Config& config)
    : AdapterFactory(&config)
{
}

AbstractAdapter* TestAdapterFactory::create(const QString& type, const QString& name)
{
    TestAdapter* adapter = new TestAdapter;
    _adapters.append(adapter);
    return adapter;
}

/**
 *@details
 */
TestAdapterFactory::~TestAdapterFactory()
{
}

} // namespace pelican
