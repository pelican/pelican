#ifndef CHUNKERFACTORY_H
#define CHUNKERFACTORY_H

#include <QString>

#include "pelican/utility/AbstractFactory.h"

/**
 * @file ChunkerFactory.h
 */

namespace pelican {

class AbstractChunker;

/**
 * @class ChunkerFactory
 *
 * @brief
 * Class to create configured chunker objects.
 *
 * @details
 * This class creates configured chunker objects that have been declared
 * using the PELICAN_DECLARE_CHUNKER macro.
 * Calling the create() method will then generate the object with the
 * appropriate configuration.
 *
 * Use the PELICAN_DECLARE_CHUNKER macro in the chunker's source (.cpp)
 * file to register the chunker with the factory.
 *
 * \code
 *
 * // Create the chunker factory.
 * Config::TreeAddress address;
 * address.append(Config::NodeId("server", ""));
 * address.append(Config::NodeId("chunkers", ""));
 * ChunkerFactory factory(&config, address);
 *
 * // Set up the factory.
 * ChunkerFactory factory;
 *
 * // Create a chunker.
 * AbstractChunker* chunker = factory.create("MyChunkerType");
 *
 * \endcode
 */
class ChunkerFactory : public AbstractFactory
{
    public:
        /// Abstract base for object creators.
        struct AbstractCreator
        {
            public:
                /// Constructs an AbstractCreator.
                AbstractCreator(const QString& type) : _type(type) {}

                /// Creates a chunker of the given type (pure virtual).
                virtual AbstractChunker* create(const ConfigNode& config) = 0;

                /// Returns a reference to the type of this creator.
                QString type() {return _type;}

                /// Returns a reference to the type map.
                static QHash<QString, AbstractCreator*>& types() {return _types;}

            private:
                QString _type;
                static QHash<QString, AbstractCreator*> _types;
        };

        /// Template for creating objects.
        template<typename Type> struct Creator : public AbstractCreator
        {
            /// Constructs a Creator for the given type.
            /// This inserts the type name and a pointer to this generator
            /// into the hash of known object types, so that it can be created.
            Creator(const QString& type) : AbstractCreator(type) {
                types().insert(type, this);
            }

            /// Allocates and constructs an object of the given type.
            AbstractChunker* create(const ConfigNode& config) {
                return new Type(config);
            }
        };

    public:
        /// Creates the chunker factory.
        ChunkerFactory(const Config* config, const Config::TreeAddress& base)
        : AbstractFactory(config, base) {}

        /// Destroys the chunker factory.
        ~ChunkerFactory();

        /// Create a configured chunker of the given type and name.
        AbstractChunker* create(const QString& type, const QString& name="");

    private:
        QList<AbstractChunker*> _objects;
};

} // namespace pelican

#endif // CHUNKERFACTORY_H
