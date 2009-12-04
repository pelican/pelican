#ifndef TESTMODULE_H
#define TESTMODULE_H

class QDomElement;

#include "AbstractModule.h"

/**
 * @file TestModule.h
 */

namespace pelican {

/**
 * @class TestModule
 *  
 * @brief
 * 
 * @details
 * 
 */

class TestModule : public AbstractModule
{
    public:
        TestModule(const QDomElement& config );
        ~TestModule();

    private:
};

} // namespace pelican
#endif // TESTMODULE_H 
