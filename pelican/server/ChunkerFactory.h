#ifndef CHUNKERFACTORY_H
#define CHUNKERFACTORY_H

#include <QString>

#include "pelican/utility/AbstractFactory.h"
#include "pelican/server/AbstractChunker.h"

/**
 * @file ChunkerFactory.h
 */

namespace pelican {

/**
 * This macro is used to register the named chunker type with the factory.
 * It should be used within the global scope of the chunker's source file.
 *
 * @note
 * The macro expands to declare a dummy variable of the object's generator
 * type, which (when constructed), adds the type name to the creator's known
 * object types.
 *
 * The macro is placed within the global scope so that it is initialised as
 * soon as the program starts executing. It is placed within an anonymous
 * namespace so that the dummy creator variable is not accessible from outside
 * the file that instantiated it.
 */
#define PELICAN_DECLARE_CHUNKER(type) namespace {ChunkerFactory::Creator<type> reg(#type);}

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
