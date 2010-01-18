#include <QCoreApplication>
#include <QString>
#include "core/PipelineApplication.h"
#include "core/CoreOptions.h"
#include "core/ModuleFactory.h"
#include "core/AbstractDataClient.h"
#include "boost/program_options.hpp"
#include <string>
#include <iostream>

#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

namespace po = boost::program_options;

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
    /* Check for QCoreApplication */
    if (QCoreApplication::instance() == NULL)
        throw QString("Create a QCoreApplication before the pipeline application.");

    /* Set configuration using command line arguments */
    _createConfig(argc, argv);

    /* Construct the module factory */
    _createModuleFactory(_config);

    /* Construct the data client */
    _createDataClient(_config);
}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
    delete _config;
    delete _factory;
    delete _dataClient;
    _config = NULL;
    _factory = NULL;
    _dataClient = NULL;
}

/**
 * @details
 * Return the configuration file name.
 */
QString PipelineApplication::getConfigFile() const
{
    return _config->getFileName();
}

/**
 * @details
 * Registers a pipeline with the pipeline driver.
 *
 * @param[in] pipeline Pointer to the allocated pipeline.
 */
void PipelineApplication::registerPipeline(AbstractPipeline *pipeline)
{
    _driver.registerPipeline(pipeline);
}

/**
 * @details
 * Starts the pipeline driver and sets pointers to the module factory and
 * data client.
 */
void PipelineApplication::start()
{
    _driver.setDataClient(_dataClient);
    _driver.setModuleFactory(_factory);
    _driver.start();
}

/**
 * @details
 * This method is called by the constructor and parses the command line arguments
 * to create the configuration object.
 */
void PipelineApplication::_createConfig(int argc, char** argv)
{
    /* Check that argc and argv are nonzero */
    if (argc == 0 || argv == NULL)
        throw QString("No command line.");

    /* Parse the command line arguments */
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("config,c", po::value<std::string>(), "set configuration file")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }

    std::string config = "";

    if (vm.count("config")) {
        std::cout << "Configuration file was set to "
        << vm["config"].as<std::string>() << ".\n";
        config = vm["config"].as<std::string>();
    } else {
        std::cout << "Configuration file not set on command line.\n";
    }

    /* Construct the configuration object */
    _config = new Config(QString::fromStdString(config));

    return;
}

/**
 * @details
 * Creates the data client based on the supplied configuration object.
 */
void PipelineApplication::_createDataClient(const Config* config)
{
    // TODO create data client.
}

/**
 * @details
 * Creates the module factory based on the supplied configuration object.
 */
void PipelineApplication::_createModuleFactory(const Config* config)
{
    _factory = new ModuleFactory(config);
}

} // namespace pelican
