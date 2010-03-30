#ifndef CHUNKERCLASSGENERATORBASE_H
#define CHUNKERCLASSGENERATORBASE_H
#include <QString>

/**
 * @file ChunkerClassGeneratorBase.h
 */

namespace pelican {
    class AbstractChunker;
    class ConfigNode;

/**
 * @class ChunkerClassGeneratorBase
 *  
 * @brief
 *    Abstract Base class for generators of Chunker objects
 *    for use in ChunkerFactory
 * @details
 *    This allows plug-ins to be introduced into the Chunker Factory
 * 
 */

class ChunkerClassGeneratorBase
{
    public:
        ChunkerClassGeneratorBase( const QString& type );
        virtual ~ChunkerClassGeneratorBase();
        QString type() { return _type; };
        virtual AbstractChunker* create(const ConfigNode& config) = 0;

    private:
        QString _type;
};

} // namespace pelican
#endif // CHUNKERCLASSGENERATORBASE_H 
