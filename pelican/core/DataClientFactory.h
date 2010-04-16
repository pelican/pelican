#ifndef DATACLIENTFACTORY_H
#define DATACLIENTFACTORY_H

#include <QList>
#include <QString>
#include "pelican/core/AbstractDataClient.h"
#include "pelican/utility/Factory.h"

/**
 * @file DataClientFactory.h
 */

namespace pelican {

class AbstractAdapter;
class DataTypes;
class DataRequirements;

/**
 * @class DataClientFactory
 *  
 * @brief
 * Construct data clients.
 *
 * @details
 * 
 */
class DataClientFactory : public Factory<AbstractDataClient>
{
    private:
        Factory<AbstractAdapter>* _adapterFactory;

    public:
        /// Constructs the data client factory.
        DataClientFactory(const Config* config,
                const Config::TreeAddress& base, Factory<AbstractAdapter>* aFactory)
        : Factory<AbstractDataClient>(config, base), _adapterFactory(aFactory) {}

        /// Create a configured object with the given name and type.
        AbstractDataClient* create(const QString& type,
                const DataTypes& requirements, const QString& name="");

        /// Create a configured object with the given name and type from a list of data requirements.
        AbstractDataClient* create(const QString& type,
                const QList<DataRequirements>& requirements, const QString& name="");

    private:
        /// Disable inherited method.
        AbstractDataClient* create(const QString&, const QString& = "") {
            return 0;
        }
};

} // namespace pelican

#endif // DATACLIENTFACTORY_H 
