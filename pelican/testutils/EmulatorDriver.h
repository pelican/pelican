#ifndef EMULATORDRIVER_H
#define EMULATORDRIVER_H

/**
 * @file EmulatorDriver.h
 */

#include "pelican/utility/ConfigNode.h"
#include "pelican/testutils/EmulatorDriver.h"
#include <QtCore/QThread>

class QIODevice;

namespace pelican {

class AbstractEmulator;

/**
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

    protected:
        /// Runs the thread owned by the emulator driver.
        void run();

    private:
        bool _abort;
        QIODevice* _device;
        AbstractEmulator* _emulator;
};

} // namespace pelican

#endif // EMULATORDRIVER_H 
