#include <QCoreApplication>
#include <QString>
#include "adapters/AdapterFactory.h"
#include "core/PipelineApplication.h"
#include "core/PipelineDriver.h"
#include "core/ModuleFactory.h"
#include "data/DataBlobFactory.h"
#include "boost/program_options.hpp"
#include <string>
#include <iostream>

#include "utility/Config.h"
#include "utility/memCheck.h"

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
    /* Initialise member variables */
    _config = NULL;
    _adapterFactory = NULL;
    _moduleFactory = NULL;
    _dataBlobFactory = NULL;
    _driver = NULL;

    /* Check for QCoreApplication */
    if (QCoreApplication::instance() == NULL)
        throw QString("Create a QCoreApplication before the pipeline application.");

    /* Set configuration using command line arguments */
    if (!_createConfig(argc, argv))
        return;

    /* Construct the module factory */
    _moduleFactory = new ModuleFactory(_config);

    /* Construct the adapter factory */
    _adapterFactory = new AdapterFactory(_config);

    /* Construct the data blob factory */
    _dataBlobFactory = new DataBlobFactory;

    /* Construct the pipeline driver */
    _driver = new PipelineDriver(_dataBlobFactory, _adapterFactory);
}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
    delete _config;
    delete _adapterFactory;
    delete _dataBlobFactory;
    delete _driver;
    delete _moduleFactory;
    _config = NULL;
    _adapterFactory = NULL;
    _dataBlobFactory = NULL;
    _driver = NULL;
    _moduleFactory = NULL;
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
 * The recognised values are:
 * - FileDataClient
 *
 * @param[in] name The type of the data client to create.
 */
void PipelineApplication::setDataClient(QString name)
{
    _driver->setDataClient(name, _config);
}

/**
 * @details
 * Starts the pipeline driver and sets pointers to the module factory and
 * data client. This should be called from main().
 */
void PipelineApplication::start()
{
    _driver->setModuleFactory(_moduleFactory);
    _driver->start();
}

/**
 * @details
 * This method is called by the constructor and parses the command line
 * arguments to create the configuration object.
 *
 * @return
 * This method returns true if the Config object was created, or false if not.
 */
bool PipelineApplication::_createConfig(int argc, char** argv)
{
    /* Check that argc and argv are nonzero */
    if (argc == 0 || argv == NULL)
        throw QString("No command line.");

    /* Declare the supported options */
    opts::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Produce help message.")
        ("config,c", opts::value<std::string>(), "Set configuration file.")
    ;

    /* Parse the command line arguments */
    opts::variables_map varMap;
    opts::store(opts::parse_command_line(argc, argv, desc), varMap);
    opts::notify(varMap);

    /* Check for help message */
    if (varMap.count("help")) {
        std::cout << desc << "\n";
        return false;
    }

    /* Get the configuration file name */
    std::string configFilename = "";
    if (varMap.count("config"))
        configFilename = varMap["config"].as<std::string>();

    /* Construct the configuration object */
    try {
        _config = new Config(QString::fromStdString(configFilename));
    } catch (QString error) {
        std::cerr << error.toLatin1().data() << std::endl;
    }

    return true;
}

} // namespace pelican
