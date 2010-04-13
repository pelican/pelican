#ifndef CHUNKERFACTORY_H
#define CHUNKERFACTORY_H

#include <QString>

#include "utility/AbstractFactory.h"
#include "server/AbstractChunker.h"

/**
 * @file ChunkerFactory.h
 */

namespace pelican {

#ifndef PELICAN_DECLARE_CHUNKER
#define PELICAN_DECLARE_CHUNKER(type) namespace {ChunkerFactory::Generator<type> reg(#type);}
#endif

/**
 * @class ChunkerFactory
 *
 * @brief
 * Creates configured Chunker objects.
 *
 * @details
 * The factory must be configured by adding ChunkerType objects
 * which can create the necessary objects.
 * Create will then generate the object, passing down in its
 * instantiator the the appropriate configuration object.
 *
 * Use the PELICAN_DECLARE_CHUNKER macro in the chunker's source (.cpp)
 * file to register the chunker with the factory.
 *
 * \code
 *
 * // Create the chunker factory.
 * Config::TreeAddress_t address;
 * address.append(Config::NodeId_t("server", ""));
 * address.append(Config::NodeId_t("chunkers", ""));
 * ChunkerFactory factory(&config, address);
 *
 * // Set up the factory.
 * ChunkerFactory factory;
 *
 * // Create a chunker.
 * AbstractChunker* chunker = factory.create("myChunker");
 *
 * \endcode
 */
class ChunkerFactory : public AbstractFactory
{
    public:
        /// Abstract base class for object generators.
        class AbstractGenerator
        {
            public:
                AbstractGenerator(const QString& type) : _type(type) {}
                virtual ~AbstractGenerator() {}
                QString type() {return _type;}
                virtual AbstractChunker* create(const ConfigNode& config) = 0;

                /// Returns a reference to the type map.
                static QHash<QString, AbstractGenerator*> types() {return _types;}

            protected:
                static QHash<QString, AbstractGenerator*> _types;

            private:
                QString _type;
        };

        /// Template class generator for constructing objects.
        template<class Type> class Generator : public AbstractGenerator
        {
            public:
                /// Constructs a new Generator for the given type.
                Generator(const QString& type) : AbstractGenerator(type) {
                    _types.insert(type, this);
                }

                /// Creates a chunker of the given type.
                AbstractChunker* create(const ConfigNode& config) {
                    return new Type(config);
                }
        };

    public:
        ChunkerFactory(const Config* config, const Config::TreeAddress_t& base);
        ~ChunkerFactory();

        /// Create a configured object with the given name and type.
        virtual AbstractChunker* create(const QString& type, const QString& name="");

    private:
        QList<AbstractChunker*> _objects;
};

} // namespace pelican

#endif // CHUNKERFACTORY_H 
