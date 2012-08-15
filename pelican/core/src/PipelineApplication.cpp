#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include "pelican/modules/AbstractModule.h"
#include "pelican/core/PipelineApplication.h"
#include "pelican/core/PipelineDriver.h"
#include "pelican/core/DataClientFactory.h"
#include "pelican/core/AbstractAdapterFactory.h"
#include "boost/program_options.hpp"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"
#include "pelican/output/OutputStreamManager.h"
#include <string>
#include <csignal>
#include <iostream>

namespace pelican {

namespace opts = boost::program_options;

QList<PipelineDriver*> PipelineApplication::_allDrivers;

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
    // Set configuration using command line arguments
    _createConfig(argc, argv);
    _init();
}

PipelineApplication::PipelineApplication(const Config& config )
{
    _config = config;
    _init();
}

void PipelineApplication::setConfigurationSearchPaths( const QList<QString>& paths ) {
    _config.setSearchPaths( paths );
}

void PipelineApplication::_init() 
{
     _adapterFactory = 0;
     _clientFactory = 0;
     _moduleFactory = 0;

    // Check for QCoreApplication
    if (QCoreApplication::instance() == NULL)
        throw QString("Create a QCoreApplication before the PipelineApplication.");

    // setup pipeline configuration node address
    _pipelineAddress <<  Config::NodeId("configuration", "");
    _pipelineAddress <<  Config::NodeId("pipeline", "");

    Config::TreeAddress pipelineConfig( _pipelineAddress );
    pipelineConfig << Config::NodeId("pipelineConfig","");

    // initialise the factories
    //_adapterFactory = new FactoryConfig<AbstractAdapter>(config(),
    //            "pipeline", "adapters");
    _adapterFactory = new AbstractAdapterFactory(config(), "pipeline", "adapters");
    _clientFactory = new DataClientFactory(config(), "pipeline", "clients",
            adapterFactory() );
    _moduleFactory = new FactoryConfig<AbstractModule>(config(), "pipeline", "modules", false);

    // Construct the pipeline driver.
    _driver = new PipelineDriver( dataBlobFactory(), _moduleFactory, _clientFactory, 
                                  outputStreamManager(), &_config, pipelineConfig );
    _allDrivers.append(_driver);

    // install signal handlers
    // to ensure we clean up properly
    signal( SIGINT, &PipelineApplication::exit );
    signal( SIGTERM, &PipelineApplication::exit);
}

void PipelineApplication::exit(int) {
    QCoreApplication::exit(0);
    foreach( PipelineDriver* d, _allDrivers ) {
        d->stop();
    }
}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
    _driver->stop();
    _allDrivers.removeAll(_driver);
    delete _driver;
    delete _adapterFactory;
    delete _clientFactory;
    delete _moduleFactory;
}

/**
 * @details
 * Returns a pointer to the application's adapter factory.
 */
AbstractAdapterFactory* PipelineApplication::adapterFactory()
{
    return _adapterFactory;
}

/**
 * @details
 * Returns a pointer to the application's data client factory.
 */
DataClientFactory* PipelineApplication::clientFactory()
{
    return _clientFactory;
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
    return &osmanager;
}

/**
 * @details
 * Returns a pointer to the application's data blob factory.
 */
FactoryGeneric<DataBlob>* PipelineApplication::dataBlobFactory()
{
    static FactoryGeneric<DataBlob> factory(false);
    return &factory;
}

/**
 * @details
 * Returns a pointer to the application's module factory.
 */
FactoryConfig<AbstractModule>* PipelineApplication::moduleFactory() const
{
    return _moduleFactory;
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

void PipelineApplication::addPipelineSwitcher(const PipelineSwitcher& switcher)
{
    _driver->addPipelineSwitcher(switcher);
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

void PipelineApplication::setDataClient(AbstractDataClient* client)
{
    _driver->setDataClient(client);
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

void PipelineApplication::stop()
{
    try {
        _driver->stop();
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

    // Construct the configuration object.
    try {
        // The static object is initialised only once.
        _config = Config(QString::fromStdString(configFilename));
    }
    catch (QString error) {
        std::cerr << error.toStdString() << std::endl;
    }
}

} // namespace pelican
