#ifndef DATACLIENTFACTORY_H
#define DATACLIENTFACTORY_H

#include <QList>
#include <QString>
#include "utility/AbstractFactory.h"

/**
 * @file DataClientFactory.h
 */

namespace pelican {
    class AbstractDataClient;
    class AdapterFactory;
    class DataTypes;
    class DataRequirements;

/**
 * @class DataClientFactory
 *  
 * @brief
 *    Construct data clients
 * @details
 * 
 */

class DataClientFactory : public AbstractFactory
{
    public:
        DataClientFactory( const Config* config, const Config::TreeAddress_t& base, AdapterFactory* );
        ~DataClientFactory();

        /// create a configured object with tthe given name and type
        AbstractDataClient* create( const QString& type, const DataTypes& requirements, const QString& name="" );

        /// create a configured object with the given name and type from a list of data requirements
        AbstractDataClient* create( const QString& type, const QList<DataRequirements>& requirements, const QString& name="");

    private:
        QList<AbstractDataClient*> _clients;
        AdapterFactory* _adapterFactory;
};

} // namespace pelican

#endif // DATACLIENTFACTORY_H 
