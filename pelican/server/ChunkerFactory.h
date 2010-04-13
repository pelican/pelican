#ifndef CHUNKERFACTORY_H
#define CHUNKERFACTORY_H

#include <QString>

#include "utility/AbstractFactory.h"
#include "server/AbstractChunker.h"
#include "server/ChunkerClassGenerator.hpp"

/**
 * @file ChunkerFactory.h
 */

namespace pelican {

/**
 * @class ChunkerFactory
 *
 * @brief
 * Creates configured chunker objects.
 *
 * @details
 * The ChunkerFactory class creates pre-configured chunkers of the required
 * type using the specified configuration object.
 *
 * \code
 *
 * // Create the chunker factory.
 * Config::TreeAddress_t address;
 * address.append(Config::NodeId_t("server", ""));
 * address.append(Config::NodeId_t("chunkers", ""));
 * ChunkerFactory factory(&config, address);
 *
 * // Create a chunker.
 * AbstractChunker* c = factory.create<TestUdpChunker>("TestUdpChunker");
 *
 * \endcode
 */
//class ChunkerFactory : public AbstractFactory
//{
//    public:
//        /// Creates the chunker factory.
//        ChunkerFactory(const Config* config, const Config::TreeAddress_t& base)
//        : AbstractFactory(config, base) {}
//
//        /// Destroys the chunker factory, deleting any chunkers created by it.
//        ~ChunkerFactory() {
//            for (int i = 0; i < _chunkers.size(); ++i)
//                delete _chunkers[i];
//        }
//
//        /// Creates a configured chunker of the given type and name.
//        template <typename T> AbstractChunker* create(const QString& configType,
//                const QString& configName = "")
//        {
//            AbstractChunker* chunker = new T(configuration(configType, configName));
//            _chunkers.append(chunker);
//            return chunker;
//        }
//
//    private:
//        QList<AbstractChunker*> _chunkers;
//};


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
 * instantiator the the appropriate configuration object
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
 * factory.registerChunkerType(new myChunkerType);
 *
 * // Create a chunker.
 * AbstractChunker* chunker = factory.create("myChunker");
*
* \endcode
*/
class ChunkerFactory : public AbstractFactory
{
    public:
        /// Creates the chunker factory.
        ChunkerFactory(const Config* config, const Config::TreeAddress& base)
        : AbstractFactory(config, base) {}

        ~ChunkerFactory();

        /// add a type of chunker to the chunker factory
        // This will take ownership of the object and delete it on destruction
        static void registerChunkerType(boost::shared_ptr<ChunkerClassGeneratorBase> ct);

        /// create a configured object with tthe given name and type
        virtual AbstractChunker* create(const QString& type, const QString& name="");

    private:
        static QHash<QString, boost::shared_ptr<ChunkerClassGeneratorBase> > _types;
};

} // namespace pelican

#endif // CHUNKERFACTORY_H
