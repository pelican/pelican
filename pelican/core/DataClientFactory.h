#ifndef DATACLIENTFACTORY_H
#define DATACLIENTFACTORY_H

/**
 * @file DataClientFactory.h
 */

#include "pelican/core/AbstractDataClient.h"
#include "pelican/core/AbstractAdapterFactory.h"
#include "pelican/utility/FactoryConfig.h"

#include <QtCore/QList>
#include <QtCore/QString>

namespace pelican {

class AbstractAdapter;
class DataTypes;
class DataRequirements;

/**
 * @ingroup c_core
 *
 * @class DataClientFactory
 *
 * @brief
 * Construct data clients.
 *
 * @details
 */

class DataClientFactory : public FactoryConfig<AbstractDataClient>
{
    private:
        AbstractAdapterFactory* _adapterFactory;

    public:
        /// Constructs the data client factory.
        DataClientFactory(const Config* config, const QString& section,
                const QString& group, AbstractAdapterFactory* aFactory)
        : FactoryConfig<AbstractDataClient>(config, section, group),
          _adapterFactory(aFactory) {}

        /// Create a configured object with the given name and type from a list
        /// of data requirements.
        AbstractDataClient* create(const QString& type,
                const QList<DataRequirements>& requirements,
                const QString& name="");

    private:
        /// Disable inherited method.
        AbstractDataClient* create(const QString&, const QString& = "")
        { return 0; }
};

} // namespace pelican
#endif // DATACLIENTFACTORY_H
