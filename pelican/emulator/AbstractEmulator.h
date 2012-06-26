#ifndef ABSTRACTEMULATOR_H
#define ABSTRACTEMULATOR_H

/**
 * @file AbstractEmulator.h
 */

#include <QtCore/QIODevice>

namespace pelican {

/**
 * @ingroup c_emulator
 *
 * @class AbstractEmulator
 *
 * @brief Interface for all emulators.
 *
 * @details
 * This abstract class defines the interface for all data emulators.
 */
class AbstractEmulator
{
    public:
        /// Constructor.
        AbstractEmulator() : _device(0) {}

        /// Destroys the abstract emulator and the device.
        virtual ~AbstractEmulator() {delete _device;}

        /// Automatically start.
        virtual bool autoStart() {return true;}

        /// Creates and opens the device ready for use.
        virtual QIODevice* createDevice() = 0;

        /// Gets one packet of data.
        virtual void getPacketData(char*& ptr, unsigned long& size) = 0;

        /// Returns the interval in microseconds between packets.
        virtual unsigned long interval() { return 100000;}

        /// Returns the number of packets to send. If negative, run forever.
        virtual int nPackets() {return -1;}

        /// Sets the device used by the abstract emulator.
        void setDevice(QIODevice* device) {_device = device;}

        /// Returns the start delay interval in seconds.
        virtual int startDelay() {return 0;}

    private:
        QIODevice* _device; ///< The output device to use.
};

} // namespace pelican

#endif // ABSTRACTEMULATOR_H
