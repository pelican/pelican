#include "PipelineApplication.h"
#include "CoreOptions.h"
#include "boost/program_options.hpp"
#include <string>
#include <iostream>
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
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("config,c", po::value<int>(), "set configuration file")
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
        << vm["config"].as<int>() << ".\n";
    } else {
        std::cout << "Configuration file was not set.\n";
    }

}

/**
 * @details
 * Destroys the pipeline application.
 */
PipelineApplication::~PipelineApplication()
{
}

} // namespace pelican
