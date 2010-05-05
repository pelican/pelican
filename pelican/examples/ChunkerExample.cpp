#include "ChunkerExample.h"
#include <QUdpSocket>

/**
 *@details ChunkerExample 
 */
ChunkerExample::ChunkerExample(const ConfigNode& config)
    : AbstractChunker(config)
{
    // set packet size from the config
    packetSize = config.getOption("packet","size");
}

ChunkerExample::~ChunkerExample()
{
}

// Creates a suitable device ready for reading
QIODevice* ChunkerExample::newDevice()
{
    // return an opened QUdpSocket
    QUdpSocket* socket = new QUdpSocket;
    QHostAddress hostAddress(host());
    socket -> bind( hostAddress, port() );
    return socket;
}

// Called whenever there is data available on the device
void ChunkerExample::next(QIODevice* device)
{
    // get some buffer space to write to
    WritableData writableData = getDataStorage( packetSize );

    while( device->bytesAvailable < packetSize ){ msleep(1) };

    if ( writableData.isValid())
    {
        writableData->write(device.read(packetSize), packetSize); //TODO
    }
    else {
        // no space left so throw this chunk away
        device.read(packetSize);
    }
}
