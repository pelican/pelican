#include "pelican/emulator/AbstractUdpEmulator.h"
#include "pelican/utility/ConfigNode.h"

#include <QtNetwork/QUdpSocket>

namespace pelican {

/**
 * @details AbstractUdpEmulator
 */
AbstractUdpEmulator::AbstractUdpEmulator(const ConfigNode& configNode)
    : AbstractEmulator()
{
    _host = QHostAddress(configNode.getOption("connection", "host",
            "127.0.0.1"));
    _port = configNode.getOption("connection", "port", "2001").toShort();
}

/**
 * @details
 * Creates an open UDP socket. The socket will be deleted by the base class.
 */
QIODevice* AbstractUdpEmulator::createDevice()
{
    QUdpSocket* socket = new QUdpSocket;
    socket->connectToHost(_host, _port);
    return socket;
}

} // namespace pelican
