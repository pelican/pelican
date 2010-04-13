#include "TestAdapterFactory.h"
#include "TestAdapter.h"
#include "TestStreamAdapter.h"

#include "utility/memCheck.h"

namespace pelican {

/**
 *@details TestAdapterFactory
 */
TestAdapterFactory::TestAdapterFactory(const pelican::Config& config)
    : AdapterFactory(&config)
{
}

AbstractAdapter* TestAdapterFactory::create(const QString& type,
        const QString& /* name */)
{
    AbstractAdapter* adapter = 0;
    if (type == "service") {
        adapter = new TestAdapter;
    }
    else {
        adapter = new TestStreamAdapter;
    }
    _adapters.append(adapter);
    return adapter;
}


/**
 * @details
 */
TestAdapterFactory::~TestAdapterFactory()
{
}

} // namespace pelican
