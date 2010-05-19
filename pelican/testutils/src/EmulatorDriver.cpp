#include "pelican/testutils/EmulatorDriver.h"
#include "pelican/testutils/AbstractEmulator.h"

#include <QtCore/QIODevice>
#include <iostream>

#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs the emulator driver and starts its thread.
 */
EmulatorDriver::EmulatorDriver(AbstractEmulator* emulator) : QThread()
{
    _abort = false;
    _emulator = emulator;

    // Start the thread.
    start();
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
    // Create the device.
    _device = _emulator->createDevice();
    _emulator->setDevice(_device); // The base class deletes the device.

    // Enter infinite loop.
    bool noData = false;
    while (!_abort && !noData) {
        // Get the data.
        char* ptr = 0;
        unsigned long size = 0;
        _emulator->getPacketData(ptr, size);
        if (ptr == 0 || size == 0) noData = true;

        // Write to the device.
        _device->write(ptr, size);

        // Sleep.
        if (_emulator->interval() != 0)
            usleep(_emulator->interval());
    }

    // Warn if no data returned.
    if (noData)
        std::cerr << "EmulatorDriver: No data to send." << std::endl;
}

} // namespace pelican
