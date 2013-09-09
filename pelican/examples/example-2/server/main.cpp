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

#include <pelican/server/PelicanServer.h>
#include <pelican/comms/PelicanProtocol.h>
#include <pelican/utility/Config.h>

#include <QtCore/QCoreApplication>

#include "StreamChunker.hpp"

#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace pelican;
using namespace std;


// Re-implementing QApplication::notify() allows capturing of
// exceptions thrown from within an event handler.
class MyApp : public QCoreApplication
{
public:
    MyApp(int argc, char** argv) : QCoreApplication(argc, argv) {}
    virtual ~MyApp() {}
    virtual bool notify(QObject* rec, QEvent* ev) {
        try {
            return QCoreApplication::notify(rec, ev);
        }
        catch (const QString& e)
        {
            cerr << "ERROR: " << e.toStdString() << endl;
            abort();
        }
        catch (...) {
            cerr << "ERROR: Unknown exception!" << endl;
            abort();
        }
        return false;
    }
};


int main(int argc, char** argv)
{
    MyApp app(argc, argv);

    if (argc != 2)
    {
        cerr << "Please specify an XML configuration file." << endl;
        return 0;
    }

    try {
        Config config(argv[1]);
        PelicanServer server(&config);
        server.addStreamChunker("StreamChunker");
        server.addProtocol(new PelicanProtocol, 2000);
        server.start();
        while (!server.isReady()) { usleep(100); }
//        while (server.isRunning()) {
//            MyApp::processEvents();
//            usleep(100);
//        }
        return app.exec();
    }
    catch (const QString& err) {
        cerr << "ERROR: " << err.toStdString() << endl;
    }

    return 0;
}
