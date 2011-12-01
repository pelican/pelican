#ifndef ABSTRACTUDPEMULATOR_H
#define ABSTRACTUDPEMULATOR_H

/**
 * @file AbstractUdpEmulator.h
 */

#include "pelican/emulator/AbstractEmulator.h"
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
