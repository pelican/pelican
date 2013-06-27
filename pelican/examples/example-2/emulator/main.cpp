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

#include <pelican/emulator/EmulatorDriver.h>
#include <pelican/utility/Config.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include "StreamEmulator.hpp"
#include <iostream>
#include <cstdio>

using namespace std;
using namespace pelican;

void usage(const char* msg = 0)
{
    if (msg) {
        cerr << endl;
        cerr << "ERROR: " << msg  << endl;
    }
    cerr << endl;
    cerr << "Usage: :" << endl;
    cerr << "  $ emulator <XML configuration file> [node name]" << endl;
    cerr << endl;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    if (argc < 2 || argc > 3) {
        usage("Please specify an XML configuration file.");
        return EXIT_FAILURE;
    }

    try {
        const char* configFile = argv[1];
        const char* nodeName = (argc == 3) ? argv[2] : "opt1";
        cout << "Running emulator ..." << endl;
        cout << " * Config. file  = " << configFile << endl;
        cout << " * Settings name = " << nodeName << endl;
        cout << endl;
        Config config(configFile);
        Config::TreeAddress address;
        address << Config::NodeId("configuration", "");
        address << Config::NodeId("StreamEmulator", nodeName);
        ConfigNode settings;
        if (config.verifyAddress(address)) {
            settings = config.get(address);
        }
        else {
            usage("Unable to find valid XML config. node in the specified XML file.");
            return EXIT_FAILURE;
        }
        EmulatorDriver emulatorDriver(new StreamEmulator(settings));
        return app.exec();
    }
    catch (const QString& err)
    {
        cerr << "ERROR: " << err.toStdString() << endl;
        usage();
    }
    return EXIT_SUCCESS;
}


