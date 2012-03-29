#ifndef ABSTRACTADAPTERFACTORY_H
#define ABSTRACTADAPTERFACTORY_H


#include "pelican/utility/FactoryConfig.h"
#include "pelican/core/AbstractAdapter.h"

/**
 * @file AbstractAdapterFactory.h
 */

namespace pelican {

/**
 * @class AbstractAdapterFactory
 *  
 * @brief
 * 
 * @details
 * 
 */

class AbstractAdapterFactory : public FactoryConfig<AbstractAdapter>
{
    public:
        AbstractAdapterFactory( const Config* config, const QString& section,
                                const QString& group );
        ~AbstractAdapterFactory();

    private:
};

} // namespace pelican
#endif // ABSTRACTADAPTERFACTORY_H 
