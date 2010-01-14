#include <QString>
#include "core/PipelineApplication.h"
#include "core/CoreOptions.h"
#include "core/ModuleFactory.h"
#include "core/DataClient.h"
#include "boost/program_options.hpp"
#include <string>
#include <iostream>

#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

namespace po = boost::program_options;

/**
 * @details
 * PipelineApplication constructor. This invokes the QCoreApplication object
 * needed for the Qt framework classes to function correctly.
 * Command line arguments are passed as function arguments.
 *
 * The PipelineApplication constructor also creates the configuration object,
 * the module factory and the data client.
 */
PipelineApplication::PipelineApplication(int argc, char** argv)
: QCoreApplication(argc, argv)
{
    _factory = 0;
    _config = 0;

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

    /* Construct the module factory */
    _factory = new ModuleFactory(_config);

    /* Construct the data client */
    _dataClient = new DataClient();
}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
    delete _config;
    delete _factory;
}

/**
 * @details
 * Return the configuration file name.
 */
QString PipelineApplication::getConfigFile() const
{
    return _config->getFileName();
}

} // namespace pelican
