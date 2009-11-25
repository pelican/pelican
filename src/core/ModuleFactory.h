#ifndef MODULEFACTORY_H
#define MODULEFACTORY_H

#include <QString>
#include <QHash>
#include <QVector>
class QDomNode;

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
 * The modules are owned by the factory and will be destroyed with
 * the factory.
 */


namespace pelican {

class AbstractModule;
class Config;

class ModuleFactory
{
    public:
        /// Module factory constructor.
        ModuleFactory(Config *config);

        /// Module factory destructor.
        ~ModuleFactory();

        /// Creates a new module.
        AbstractModule* createModule(const QString& name);

    private:
        /// Holds pointers to the created modules.
        QHash<QString, AbstractModule*> modules;

        /// Creates a new module.
        AbstractModule* _createModule(const QString& name, const QDomNode& config);

    private:
        Config* _config; ///< Pointer to the configuration object.
        QVector<AbstractModule*> _modules; // 
};

} // namespace pelican
#endif // MODULEFACTORY_H 
