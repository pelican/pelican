/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * A DataBlob relay supporting the basic Pelican Output types
 */
#include <QtCore/QCoreApplication>
#include <boost/program_options.hpp>
#include "utility/Config.h"
#include <iostream>
#include "DataBlobRelay.h"

namespace opts = boost::program_options;
using namespace pelican;

Config createConfig(const opts::variables_map&);
opts::variables_map process_options(int argc, char** argv);

int main( int argc, char** argv ) {
    // parse the args
    opts::variables_map options = process_options(argc,argv);

    // start up the relay
    int rv = 0;
    DataBlobRelay* relay = 0;
    try {
        Config config = createConfig(options);
        Config::TreeAddress address;
        address << Config::NodeId("PelicanRelay", "");

        QCoreApplication app(argc, argv );
        relay = new DataBlobRelay( &config, address );
        rv = app.exec();
    }
    catch(const boost::program_options::error& err) {
        std::cerr << "pelicanRelay ERROR: " << err.what() << std::endl;
    }
    catch (const QString& err)
    {
        std::cerr << "pelicanRelay ERROR: " << err.toStdString() << std::endl;
        rv = 1;
    }
    delete relay;
    return rv;

}

/**
 * @details
 * Create a Configuration XML document for the relay
 */
opts::variables_map process_options(int argc, char** argv)
{
    // Check that argc and argv are nonzero
    if (argc == 0 || argv == NULL) throw QString("No command line.");

    // Declare the supported options.
    opts::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Produce help message.")
        ("config,c", opts::value<std::string>(), "Set configuration file.");

    // Parse the command line arguments.
    opts::variables_map varMap;
    opts::store(opts::command_line_parser(argc, argv).options(desc)
            .run(), varMap);
    opts::notify(varMap);

    // Check for help message.
    if (varMap.count("help")) {
        std::cout << desc << std::endl;
        exit(0);
    }
    return varMap;
}

Config createConfig(const opts::variables_map& varMap)
{
    // Get the configuration file name.
    std::string configFilename = "";
    if (varMap.count("config"))
        configFilename = varMap["config"].as<std::string>();

    pelican::Config config;
    if (!configFilename.empty()) {
        config = pelican::Config(QString(configFilename.c_str()));
    } else {
        throw QString("must specify a configuration file");
    }
    return config;
}
