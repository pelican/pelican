#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include "pelican/modules/AbstractModule.h"
#include "pelican/core/PipelineApplication.h"
#include "pelican/core/PipelineDriver.h"
#include "pelican/core/DataClientFactory.h"
#include "boost/program_options.hpp"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/output/OutputStreamManager.h"
#include <string>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

namespace opts = boost::program_options;

/**
 * @details
 * PipelineApplication constructor. This requires a QCoreApplication object
 * to be created first, which is needed for the Qt framework classes to function
 * correctly. Command line arguments are passed as function arguments.
 *
 * The PipelineApplication constructor also creates the configuration object,
 * the module factory and the data client.
 */
PipelineApplication::PipelineApplication(int argc, char** argv)
{
    // Check for QCoreApplication
    if (QCoreApplication::instance() == NULL)
        throw QString("Create a QCoreApplication before the PipelineApplication.");

    // Set configuration using command line arguments
    _createConfig(argc, argv);
    if (!config())
        throw QString("Configuration object does not exist.");

    // Construct the pipeline driver.
    _driver = new PipelineDriver( dataBlobFactory(), moduleFactory(),
            clientFactory(), outputStreamManager() );
}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
    delete _driver;
}

/**
 * @details
 * Returns a pointer to the application's adapter factory.
 */
FactoryConfig<AbstractAdapter>* PipelineApplication::adapterFactory()
{
    static FactoryConfig<AbstractAdapter> factory(config(),
            "pipeline", "adapters");
    return &factory;
}

/**
 * @details
 * Returns a pointer to the application's data client factory.
 */
DataClientFactory* PipelineApplication::clientFactory()
{
    static DataClientFactory factory(config(), "pipeline", "clients",
            adapterFactory() );
    return &factory;
}

/**
 * @details
 * Returns a pointer to the application's data output stream manager.
 */
OutputStreamManager* PipelineApplication::outputStreamManager()
{
    Config::TreeAddress address;
   address <<  Config::NodeId("configuration", "");
   address <<  Config::NodeId("pipeline", "");
   address <<  Config::NodeId("output", "");
    static OutputStreamManager osmanager( config(), address );
            //Config::TreeAddress() <<  Config::NodeId("output", "") );
    return &osmanager;
}

/**
 * @details
 * Returns a pointer to the application's configuration object.
 *
 * @param configFilename The filename to use when constructing the object.
 * @return
 */
Config* PipelineApplication::config(std::string configFilename)
{
    try {
        // The static object is initialised only once.
        static Config config(QString::fromStdString(configFilename));
        return &config;
    }
    catch (QString error) {
        std::cerr << error.toStdString() << std::endl;
        return 0;
    }
}

/**
 * @details
 * Returns a pointer to the application's data blob factory.
 */
FactoryGeneric<DataBlob>* PipelineApplication::dataBlobFactory()
{
    static FactoryGeneric<DataBlob> factory;
    return &factory;
}

/**
 * @details
 * Returns a pointer to the application's module factory.
 */
FactoryConfig<AbstractModule>* PipelineApplication::moduleFactory()
{
    static FactoryConfig<AbstractModule> factory(config(), "pipeline", "modules");
    return &factory;
}

/**
 * @details
 * Registers a pipeline with the pipeline driver, which takes ownership of
 * the pipeline.
 *
 * @param[in] pipeline Pointer to the allocated pipeline.
 */
void PipelineApplication::registerPipeline(AbstractPipeline *pipeline)
{
    _driver->registerPipeline(pipeline);
}

/**
 * @details
 * Sets (and creates) the given data client based on the named argument.
 * Throws an exception of type QString if the data client is unknown.
 *
 * @param[in] name The type of the data client to create.
 */
void PipelineApplication::setDataClient(const QString& name)
{
    _driver->setDataClient(name);
}

/**
 * @details
 * Starts the pipeline driver. This should be called from main().
 */
void PipelineApplication::start()
{
    try {
        _driver->start();
    }
    catch (QString err) {
        std::cout << "ERROR: " << err.toStdString() << std::endl;
    }

}

bool PipelineApplication::isRunning() const
{
    if( _driver ) return _driver->isRunning();
    return false;
}

/**
 * @details
 * This method is called by the constructor and parses the command line
 * arguments to create the configuration object.
 */
void PipelineApplication::_createConfig(int argc, char** argv)
{
    // Check that argc and argv are nonzero
    if (argc == 0 || argv == NULL)
        throw QString("PipelineApplication: no config file or other options specified.");

    // Declare the supported options.
    opts::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Produce help message.")
        ("config,c", opts::value<std::string>(), "Set configuration file.")
    ;

    // A string without a selection flag in the first argument position is
    // assumed to be a config file.
    opts::positional_options_description p;
    p.add("config", -1);

    // Parse the command line arguments.
    opts::variables_map varMap;
    opts::store(opts::command_line_parser(argc, argv).options(desc)
            .positional(p).run(), varMap);
    opts::notify(varMap);

    // Check for help message.
    if (varMap.count("help")) {
        std::cout << desc << "\n";
        return;
    }

    // Get the configuration file name.
    std::string configFilename = "";
    if (varMap.count("config"))
        configFilename = varMap["config"].as<std::string>();

    // Check if no configuration file is supplied.
    if (configFilename.empty()) {
        std::cout << "WARNING: No configuration file supplied." << std::endl;
    }

    // Construct the static configuration object.
    if (!config(configFilename))
        throw QString("PipelineApplication: Cannot create configuration.");
}

} // namespace pelican
