#include "PipelineApplication.h"
#include "CoreOptions.h"
#include "boost/program_options.hpp"
#include <QString>
#include <string>
#include <iostream>
#include "ModuleFactory.h"
#include "utility/Config.h"
#include "utility/memCheck.h"

namespace pelican {

namespace po = boost::program_options;

/**
 * @details
 * PipelineApplication constructor.
 * Command line arguments are passed as function arguments.
 */
PipelineApplication::PipelineApplication(int argc, char** argv)
{
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

    if (vm.count("config")) {
        std::cout << "Configuration file was set to "
        << vm["config"].as<std::string>() << ".\n";
    } else {
        std::cout << "Configuration file was not set.\n";
    }

    /* Construct the configuration object */
    _config = new Config(QString::fromStdString(vm["config"].as<std::string>()));

    /* Construct the module factory */
    _factory = new ModuleFactory(_config);
}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
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
