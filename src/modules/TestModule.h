#ifndef TESTMODULE_H
#define TESTMODULE_H


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
        TestModule(const QDomNode& config );
        ~TestModule();

    private:
};

} // namespace pelican
#endif // TESTMODULE_H 
