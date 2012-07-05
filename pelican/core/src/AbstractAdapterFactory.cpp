#include "AbstractAdapterFactory.h"


namespace pelican {


/**
 *@details AbstractAdapterFactory 
 */
AbstractAdapterFactory::AbstractAdapterFactory( const Config* config, const QString& section,
                const QString& group )
    : FactoryConfig<AbstractAdapter>( config, section, group )
{
}

/**
 *@details
 */
AbstractAdapterFactory::~AbstractAdapterFactory()
{
}

} // namespace pelican
