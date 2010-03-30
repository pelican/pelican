#ifndef CHUNKERFACTORY_H
#define CHUNKERFACTORY_H

#include <QHash>
#include <boost/shared_ptr.hpp>

#include "utility/AbstractFactory.h"

/**
 * @file ChunkerFactory.h
 */

namespace pelican {
    class ChunkerClassGeneratorBase;
    class AbstractChunker;

/**
 * @class ChunkerFactory
 *  
 * @brief
 *    Creates configured Chunker objects
 * @details
 *    The factory must be configured by adding ChunkerType obejcts
 *    which can create the necessary objects.
 *    Create will then generate the object, passing down in its
 *    instantiator the the appropriate configuration object
 *
 *    e.g.
 *
 *    // set up the factory
 *    ChunkerFactory factory;
 *    factory.addType(new myChunkerType);
 *
 *    // use the factory
 *    AbstractChunker* mychunker = factory.create("myChunker");
 */

class ChunkerFactory : public AbstractFactory
{
    public:
        ChunkerFactory( const Config* config, const Config::TreeAddress_t& base );
        ~ChunkerFactory();

        /// add a type of chunker to the chunker factory
        //  This will take ownership of the object and delete it on destruction
        static void registerChunkerType( boost::shared_ptr<ChunkerClassGeneratorBase> ct );

        /// create a configured object with tthe given name and type
        virtual AbstractChunker* create( const QString& type, const QString& name="" );

    private:
        static QHash<QString, boost::shared_ptr<ChunkerClassGeneratorBase> > _types;
};

} // namespace pelican
#endif // CHUNKERFACTORY_H 
