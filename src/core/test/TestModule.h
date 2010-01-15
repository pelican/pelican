#ifndef TESTMODULE_H
#define TESTMODULE_H

class QDomElement;

#include "core/AbstractModule.h"

/**
 * @file TestModule.h
 */

namespace pelican {

/**
 * @class TestModule
 *  
 * @brief
 * Creates a test Pelican pipeline module.
 * 
 * @details
 * This class creates a test module which requires "TestStream"
 * and "TestService" data.
 */
class TestModule : public AbstractModule
{
    public:
        /// Creates a test module.
        TestModule(const QDomElement& config);

        /// Destroys the test module.
        ~TestModule();

        /// Runs the module.
        void run() {}

    private:
};

} // namespace pelican

#endif // TESTMODULE_H 
