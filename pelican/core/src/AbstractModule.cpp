#include "AbstractModule.h"
#include "pelican/core/AbstractPipeline.h"


namespace pelican {


/**
 *@details AbstractModule 
 */
AbstractModule::AbstractModule( const ConfigNode& config )
    : _config(config)
{
}

/**
 *@details
 */
AbstractModule::~AbstractModule()
{
}

DataBlob* AbstractModule::createBlob(const QString& type) const { 
    return _pipeline->createBlob(type);
}

void AbstractModule::dataOutput( const DataBlob* d,
                                 const QString& stream ) const
{
    return _pipeline->dataOutput( d, stream );
}

} // namespace pelican
