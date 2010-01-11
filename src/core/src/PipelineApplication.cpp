#include <QString>
#include <QCoreApplication>
#include "PipelineApplication.h"
#include "CoreOptions.h"
#include "boost/program_options.hpp"
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
    _factory = 0;
    _config = 0;

    /* Check that argc and argv are nonzero */
    if (argc == 0 || argv == NULL)
        throw QString("No command line.");

    /* We need a QCoreApplication object for the
     * Qt framework classes to function correctly
     * There can only be one instance of this, 
     * and we leave it to the user of the class to
     * instantiate it beforehand.
     */
    QCoreApplication* app = QCoreApplication::instance();
    if( ! app ) {
        throw QString("You must instantiate a QApplication or QCoreApplication object before PipelineApplication.");
    }

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
