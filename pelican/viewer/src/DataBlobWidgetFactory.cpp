#include "DataBlobWidgetFactory.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

namespace pelican {


/**
 * @details Constructs a DataBlobWidgetFactory object.
 */
DataBlobWidgetFactory::DataBlobWidgetFactory( const Config* config , const Config::TreeAddress& base )
    : FactoryConfig<DataBlobWidget>(config,base, /*owner=*/false)
{
}

/**
 * @details Destroys the DataBlobWidgetFactory object.
 */
DataBlobWidgetFactory::~DataBlobWidgetFactory()
{
}

DataBlobWidget* DataBlobWidgetFactory::create(const QString& type)
{
    return FactoryConfig<DataBlobWidget>::create(type, (QWidget*)0);
}

} // namespace pelican
