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

#include "emulator/EmulatorDriver.h"
#include "emulator/AbstractEmulator.h"

#include <QtCore/QIODevice>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QTime>

#include <typeinfo>
#include <iostream>
#include <unistd.h>

using namespace std;

namespace pelican {

/**
 * @details
 * Constructs the emulator driver and starts its thread.
 */
EmulatorDriver::EmulatorDriver(AbstractEmulator* emulator)
: QThread(), _abort(false), _device(0), _emulator(emulator),
  _dataCount(0), _packetCount(0)
{
    // Start the thread if required.
    if (_emulator->autoStart())
        start(QThread::LowPriority);
}

/**
 * @details
 * Destroys the emulator driver, aborting its thread.
 */
EmulatorDriver::~EmulatorDriver()
{
    // Wait for the thread to finish.
    wait();

    // Delete the emulator.
    if (_emulator)
        delete _emulator;
}

/**
 * @details
 * Runs the thread owned by the emulator driver.
 */
void EmulatorDriver::run()
{
//    QTime correctionTimer;
    try {
        // Create the device.
        _device = _emulator->createDevice();
        _emulator->setDevice(_device); // The base class deletes the device.
        sleep(_emulator->startDelay());

        // Set up loop conditions.
        bool continuous = _emulator->nPackets() < 0;
        _packetCount = 0;
        _dataCount = 0;

        // Enter loop.
        while (!_abort && (_packetCount < _emulator->nPackets() || continuous))
        {
//            correctionTimer.restart();

            // Get the data.
            char* ptr = 0;
            unsigned long size = 0;
            _emulator->getPacketData(ptr, size);
            if (ptr == 0 || size == 0)
                break;

            _dataCount += size;

            // Write to the device.

            _device->write(ptr, size);

            // Block until all data has been written to the device.
            if (typeid(*_device) == typeid(QTcpSocket)) {
                while (_device->bytesToWrite() > 0) {
                    if (static_cast<QAbstractSocket*>(_device)->state()
                            == QAbstractSocket::UnconnectedState) {
                        cerr << "Server disconnected, aborting emulator" << endl;
                        _abort = true;
                        break;
                    }
                    _device->waitForBytesWritten(-1);
                }
            }
            else {
                while (_device->bytesToWrite() > 0)
                    _device->waitForBytesWritten(-1);
            }

            // Sleep for the specified interval, correcting for time taken
            // in creating and sending the data.
            unsigned long interval = _emulator->interval();
            if (interval != 0) {
                // Correct interval for time taken in sending the data.
//                int correction = correctionTimer.elapsed()*1000;
//                usleep(interval - correction);
                usleep(interval);
            }
            ++_packetCount;

#if QT_VERSION >= 0x040300
            yieldCurrentThread();
#endif
        }
        if (!_abort)
            _emulator->emulationFinished();
    }
    catch( QString& e )
    {
        std::cerr << std::endl;
        std::cerr << "ERROR: EmulatorDriver::run() Caught exception.";
        std::cerr << "ERROR: " << e.toStdString() << std::endl;
        exit(1);
    }
}

} // namespace pelican
