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

#ifndef ABSTRACTUDPEMULATOR_H
#define ABSTRACTUDPEMULATOR_H

/**
 * @file AbstractUdpEmulator.h
 */

#include "emulator/AbstractEmulator.h"
#include <QtNetwork/QHostAddress>

class QIODevice;

namespace pelican {

class ConfigNode;

/**
 * @ingroup c_emulator
 *
 * @class AbstractUdpEmulator
 *
 * @brief A data packet emulator that uses a UDP socket.
 *
 * @details
 * This emulator uses a UDP socket as its output device.
 * It should be constructed with a configuration node that contains
 * a hostname and a port number using a connection tag.
 *
 * The default values are:
 *
 * @verbatim <connection host="127.0.0.1" port="2001" /> @endverbatim
 */
class AbstractUdpEmulator : public AbstractEmulator
{
    public:
        /// Constructs the abstract UDP data emulator.
        AbstractUdpEmulator(const ConfigNode& configNode);

        /// Destroys the abstract UDP data emulator
        virtual ~AbstractUdpEmulator() {}

        /// Creates an open UDP socket.
        QIODevice* createDevice();

    private:
        QHostAddress _host;
        qint16 _port;
};

} // namespace pelican

#endif // ABSTRACTUDPEMULATOR_H
