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

#include "emulator/test/RealUdpEmulator.h"
#include "emulator/EmulatorDriver.h"

#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

using namespace pelican;
using test::RealUdpEmulator;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    // Get command line arguments.
    if (argc != 5) {
        std::cerr << "Usage: realUdpEmulator <port> <host> "
                "<interval, microsec> <packet size, bytes>" << std::endl;
        return 1;
    }
    QString port = QString::fromAscii(argv[1]);
    QString host = QString::fromAscii(argv[2]);
    QString interval = QString::fromAscii(argv[3]);
    QString size = QString::fromAscii(argv[4]);

    // Start the emulator.
    ConfigNode emulatorConfig(
            "<RealUdpEmulator>"
            "    <connection host=\"" + host + "\" port=\"" + port + "\"/>"
            "    <packet size=\"" + size + "\" interval=\"" + interval + "\"/>"
            "</RealUdpEmulator>"
            );
    EmulatorDriver emulator(new RealUdpEmulator(emulatorConfig));

    return app.exec();
}
