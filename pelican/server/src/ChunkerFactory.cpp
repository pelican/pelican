#include "pelican/server/ChunkerFactory.h"

#include "pelican/utility/memCheck.h"

namespace pelican {

QHash<QString, ChunkerFactory::AbstractCreator* > ChunkerFactory::AbstractCreator::_types;

/**
 * @details
 * Destroys the chunker factory and deletes any chunkers created by it.
 */
ChunkerFactory::~ChunkerFactory()
{
    for (int i = 0; i < _objects.size(); ++i) {
        delete _objects[i];
    }
}

/**
 * @details
 * Creates a new abstract chunker of the given type, with the given
 * name (default blank).
 *
 * @param[in] type The chunker type.
 * @param[in] name The name of the chunker.
 *
 * @return Returns a pointer to the allocated chunker.
 */
AbstractChunker* ChunkerFactory::create(const QString& type, const QString& name)
{
    if (! AbstractCreator::types().contains(type))
        throw QString("ChunkerFactory::create(): Unknown type '%1'").arg(type);

    AbstractChunker* chunker = AbstractCreator::types()[type]->create(configuration(type, name));
    _objects.append(chunker);
    return chunker;
}

} // namespace pelican
