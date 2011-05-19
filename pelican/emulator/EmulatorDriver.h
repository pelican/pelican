#ifndef EMULATORDRIVER_H
#define EMULATORDRIVER_H

/**
 * @file EmulatorDriver.h
 */

#include "pelican/emulator/EmulatorDriver.h"
#include "pelican/utility/ConfigNode.h"
#include <QtCore/QThread>

class QIODevice;

namespace pelican {

class AbstractEmulator;

/**
 * @ingroup c_emulator
 *
 * @class EmulatorDriver
 *
 * @brief Drives the data emulator using its own thread.
 *
 * @details
 * This class is constructed with a pointer to the data emulator to use.
 * The emulator driver takes ownership of the emulator, and will delete
 * it on destruction.
 *
 * The thread created by this class repeatedly calls getPacketData() on the
 * emulator, and then sleeps for the specified interval.
 */
class EmulatorDriver : public QThread
{
    Q_OBJECT

    public:
        /// Constructor.
        EmulatorDriver(AbstractEmulator* emulator);

        /// Destructor.
        ~EmulatorDriver();

        // return the emulator object
        AbstractEmulator* emulator() const { return _emulator; };

        // return the number of bytes that have been sent
        unsigned long dataCount() const { return _dataCount; };

    protected:
        /// Runs the thread owned by the emulator driver.
        void run();

    private:
        bool _abort;
        QIODevice* _device;
        AbstractEmulator* _emulator;
        unsigned long _dataCount;
};

} // namespace pelican

#endif // EMULATORDRIVER_H
