#include "pelican/server/ChunkerFactory.h"
#include "pelican/server/ChunkerClassGeneratorBase.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

QHash<QString, boost::shared_ptr<ChunkerClassGeneratorBase> > ChunkerFactory::_types;


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
