#include "DataBlobFactory.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

namespace pelican {

/**
 * @details Constructs a DataBlobFactory object.
 */
DataBlobFactory::DataBlobFactory( )
    : FactoryGeneric<DataBlob>(/*owner=*/false)
{
}

/**
 * @details Destroys the DataBlobFactory object.
 */
DataBlobFactory::~DataBlobFactory()
{
}

} // namespace pelican
