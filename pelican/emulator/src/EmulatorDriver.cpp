#include "emulator/EmulatorDriver.h"
#include "emulator/AbstractEmulator.h"

#include <QtCore/QIODevice>
#include <QtCore/QCoreApplication>
#include <iostream>

namespace pelican {

/**
 * @details
 * Constructs the emulator driver and starts its thread.
 */
EmulatorDriver::EmulatorDriver(AbstractEmulator* emulator) : QThread()
{
    _abort = false;
    _emulator = emulator;
    _dataCount = 0;
    _device = 0;

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
    _abort = true;
    wait();

    // Delete the emulator.
    delete _emulator;
}

/**
 * @details
 * Runs the thread owned by the emulator driver.
 */
void EmulatorDriver::run()
{
    try {
        // Create the device.
        _device = _emulator->createDevice();
        _emulator->setDevice(_device); // The base class deletes the device.
        sleep(_emulator->startDelay());

        // Set up loop conditions.
        bool continuous = _emulator->nPackets() < 0;
        long int packetCounter = 0;
        _dataCount = 0;

        // Enter loop.
        while (!_abort && (packetCounter < _emulator->nPackets() || continuous))
        {
            // Get the data.
            char* ptr = 0;
            unsigned long size = 0;
            _emulator->getPacketData(ptr, size);
            if (ptr == 0 || size == 0)
                break;

            _dataCount += size;

            // Write to the device.
            _device->write(ptr, size);
            _device->waitForBytesWritten(100);

            // Sleep.
            unsigned long interval = _emulator->interval();
            if (interval != 0) usleep(interval);
            ++packetCounter;
#if QT_VERSION >= 0x040300
            yieldCurrentThread();
#endif
        }
        if (!_abort)
            _emulator->emulationFinished();
    }
    catch( QString& e )
    {
        std::cerr << "EmulaterDriver: Caught error " << e.toStdString() << std::endl;
        exit(1);
    }
}

} // namespace pelican
