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
        /// FIXME better design for setting this needed
        virtual int startDelay() {return 0;}

        /// Called just before the emulator driver exits.
        virtual void emulationFinished() {}

    private:
        QIODevice* _device; ///< The output device to use.
};

} // namespace pelican

#endif // ABSTRACTEMULATOR_H
