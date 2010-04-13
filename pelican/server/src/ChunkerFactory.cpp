#include "pelican/server/ChunkerFactory.h"
#include "pelican/server/ChunkerClassGeneratorBase.h"
#include <QtGlobal>

#include "pelican/utility/memCheck.h"

namespace pelican {

QHash<QString, ChunkerFactory::AbstractGenerator* > ChunkerFactory::AbstractGenerator::_types;

/**
 * @details ChunkerFactory
 */
ChunkerFactory::ChunkerFactory(const Config* config, const Config::TreeAddress& base)
    : AbstractFactory(config, base)
{
}

/**
 * @details
 */
ChunkerFactory::~ChunkerFactory()
{
    foreach (AbstractChunker* chunker, _objects) {
        delete chunker;
    }
}

/**
 * @details
 * @param type
 * @param name
 * @return
 */
AbstractChunker* ChunkerFactory::create(const QString& type, const QString& name)
{
    if (! AbstractGenerator::types().contains(type))
        throw QString("ChunkerFactory::create(): Unknown type \"%1\"").arg(type);

    AbstractChunker* chunker = AbstractGenerator::types()[type]->create(configuration(type, name));
    _objects.append(chunker);
    return chunker;
}

} // namespace pelican
