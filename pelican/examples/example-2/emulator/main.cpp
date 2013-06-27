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
#include <pelican/utility/ConfigNode.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QObject>

#include "StreamEmulator.hpp"

using pelican::ConfigNode;
using pelican::EmulatorDriver;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    // Emulator configuration.
    QString host = "127.0.0.1";
    quint16 port = 2000;
    QString xml = QString(
            "<StreamEmulator>"
            "   <connection host=\"%1\" port=\"%2\" />"
            "</StreamEmulator>").arg(host).arg(port);
    ConfigNode config(xml);

    StreamEmulator* emulator = new StreamEmulator(config);
    EmulatorDriver emulatorDriver(emulator);
//    QObject::connect(&emulatorDriver, SIGNAL(finished()),
//            &app, SLOT(quit()));

    return app.exec();
}


