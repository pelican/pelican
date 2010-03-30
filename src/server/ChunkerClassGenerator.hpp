#ifndef CHUNKERCLASSGENERATOR_H
#define CHUNKERCLASSGENERATOR_H
#include <QString>
#include "ChunkerFactory.h"
#include "ChunkerClassGeneratorBase.h"
#ifndef DECLARE_CHUNKER
#define PELICAN_DECLARE_CHUNKER(type) ChunkerFactory::registerChunkerType( boost::shared_ptr<ChunkerClassGenerator<type> >( new ChunkerClassGenerator<type>(\"type\") ) );
#endif

/**
 * @file ChunkerClassGenerator.hpp
 */

namespace pelican {
    class AbstractChunker;
    class ConfigNode;

/**
 * Template class generator for contructing Chunkers
 */
template<class ChunkerType>
class ChunkerClassGenerator : public ChunkerClassGeneratorBase
{
    ChunkerClassGenerator(const QString& type) : ChunkerClassGeneratorBase(type) {};
    AbstractChunker* create(const ConfigNode& config) {
        return new ChunkerType(type(), config);
    }
}

} // namespace pelican
#endif // CHUNKERCLASSGENERATOR_H 
