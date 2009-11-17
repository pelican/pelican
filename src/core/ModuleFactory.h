#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <QString>
#include <QHash>

/**
 * @file ModuleFactory.h
 */

/**
 * @class ModuleFactory
 *  
 * @brief
 * Factory to create pre-configured modules.
 * 
 * @details
 * Instantiates module objects and configures them with details taken
 * from the appropriate configuration file and/or command line.
 */

namespace pelican {

class AbstractModule;
class Config;

class ModuleFactory
{
    public:
        ModuleFactory(  );
        ~ModuleFactory();

        /// Creates a new module.
        AbstractModule* createModule(const QString& name);

    private:
        /// Holds pointers to the created modules.
        QHash<QString, AbstractModule*> modules;

        /// Creates a new module.
        void _createModule(const QString& name, const Config& config);
};

} // namespace pelican
#endif // MODULEFACTORY_H 
