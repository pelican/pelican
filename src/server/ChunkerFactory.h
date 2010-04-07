#ifndef CHUNKERFACTORY_H
#define CHUNKERFACTORY_H

#include <QString>

#include "utility/AbstractFactory.h"
#include "server/AbstractChunker.h"

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
class ChunkerFactory : public AbstractFactory
{
    public:
        /// Creates the chunker factory.
        ChunkerFactory(const Config* config, const Config::TreeAddress_t& base)
        : AbstractFactory(config, base) {}

        /// Destroys the chunker factory, deleting any chunkers created by it.
        ~ChunkerFactory() {
            for (int i = 0; i < _chunkers.size(); ++i)
                delete _chunkers[i];
        }

        /// Creates a configured chunker of the given type and name.
        template <typename T> AbstractChunker* create(const QString& configType,
                const QString& configName = "")
        {
            AbstractChunker* chunker = new T(configuration(configType, configName));
            _chunkers.append(chunker);
            return chunker;
        }

    private:
        QList<AbstractChunker*> _chunkers;
};

} // namespace pelican

#endif // CHUNKERFACTORY_H 
