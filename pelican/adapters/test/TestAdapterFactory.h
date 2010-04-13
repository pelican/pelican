#ifndef TESTADAPTERFACTORY_H
#define TESTADAPTERFACTORY_H


#include "AdapterFactory.h"
#include "pelican/utility/Config.h"

/**
 * @file TestAdapterFactory.h
 */

namespace pelican {

/**
 * @class TestAdapterFactory
 *
 * @brief
 *    Generate test adapters
 * @details
 *
 */

class TestAdapterFactory : public AdapterFactory
{
    public:
        TestAdapterFactory( const Config& = Config() );
        ~TestAdapterFactory();
        virtual AbstractAdapter* create(const QString& type,
                const QString& name = QString());

    private:
};

} // namespace pelican
#endif // TESTADAPTERFACTORY_H
