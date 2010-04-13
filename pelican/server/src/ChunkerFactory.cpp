#include "ChunkerFactory.h"
#include "ChunkerClassGeneratorBase.h"

#include "utility/memCheck.h"

namespace pelican {

QHash<QString, boost::shared_ptr<ChunkerClassGeneratorBase> > ChunkerFactory::_types;

/**
 *@details ChunkerFactory
 */
ChunkerFactory::ChunkerFactory( const Config* config, const Config::TreeAddress_t& base )
    : AbstractFactory(config, base)
{
}

/**
 *@details
 */
ChunkerFactory::~ChunkerFactory()
{
}

void ChunkerFactory::registerChunkerType( boost::shared_ptr<ChunkerClassGeneratorBase> ct )
{
    _types.insert(ct->type(), ct);
}

AbstractChunker* ChunkerFactory::create( const QString& type, const QString& name )
{
    if( ! _types.contains(type) )
    {
        throw( QString("ChunkerFactory::create(): unknown type \"") + type + QString("\"")  );
    }
    return _types[type]->create( configuration( type, name) );
}


} // namespace pelican
