#include "LofarChunker.h"
#include "LofarUdpHeader.h"
#include "LofarTypes.h"
#include <QUdpSocket>

#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

/**
 * @details
 * Constructs a new LofarChunker.
 */
LofarChunker::LofarChunker(const QString& type, QString host, quint16 port)
: AbstractChunker(type, host, port)
{
    // TODO make configurable
    _nPackets = 1;
    _packetsAccepted = 0;
    _packetsRejected = 0;
    _samplesPerPacket = 0;
    _startTime;
}

/**
 * @details
 * Constructs a new device.
 */
QIODevice* LofarChunker::newDevice()
{
    QUdpSocket* socket = new QUdpSocket;
    _device = socket;
    QHostAddress hostAddress(host());
    socket->bind( hostAddress, port() );
    return socket;
}

/**
 * @details
 * Gets the next chunk of data from the UDP socket (if it exists).
 */
void LofarChunker::next(QIODevice*)
{
    QUdpSocket *socket = static_cast<QUdpSocket*>(_device);

    int packetSize = sizeof(UDPPacket);
    size_t offset = 0;
    UDPPacket currPacket;
    UDPPacket emptyPacket;
    generateEmptyPacket(emptyPacket);
    unsigned    previousSeqid       = 0;
    TYPES::TimeStamp   actualStamp  = _samplesPerPacket;

    std::cout << "packetSize: " << packetSize << ", nPackets: " << _nPackets << std::endl;
    WritableData writableData = getDataStorage(_nPackets * packetSize);
    if (! writableData.isValid())
        throw QString("LofarChunker::next(): Writable data not valid.");

    std::cout << "LofarChunker::next()" << std::endl;

    // Loop over UDP packets.
    for (int i = 0; i < _nPackets; i++) {

        qint64 sizeDatagram;
        // Interruptible read, to allow stopping this thread even if the station does not send data
        socket->waitForReadyRead();
        if ( ( sizeDatagram = socket->read(reinterpret_cast<char*>(&currPacket), packetSize) ) <= 0 ) {
            printf("LofarChunker::next(): Error while receiving UDP Packet\n");
            continue;
        }

        // TODO Check for endianness
        ++_packetsAccepted;
        unsigned seqid   = currPacket.header.timestamp;
        unsigned blockid = currPacket.header.blockSequenceNumber;

        // If the seconds counter is 0xFFFFFFFF, the data cannot be trusted
        if (seqid == ~0U) {
            ++_packetsRejected;
            writableData.write(reinterpret_cast<void*>(&emptyPacket), packetSize, offset);
            offset += packetSize;
            continue;
        }

        // Check that the packets are contiguous
        if (previousSeqid + 1 != seqid) {
            std::cout << "Here 1  seqid " << seqid << std::endl;
            unsigned lostPackets = seqid - previousSeqid;

            // Generate lostPackets empty packets
            for (unsigned packetCounter = 0; packetCounter < lostPackets; packetCounter++) {
                writableData.write(reinterpret_cast<void*>(&emptyPacket), packetSize, offset);
                offset += packetSize;
            }

            i += lostPackets;
            previousSeqid = seqid + lostPackets;
            continue;
        }


        previousSeqid = seqid;
        if (i == 0)
            actualStamp.setStamp(seqid, blockid);

        std::cout << "Here 2" << std::endl;
        // Write the data.
        writableData.write(reinterpret_cast<void*>(&currPacket), packetSize, offset);
        offset += packetSize;
    }

}

/**
 * @details
 * Generates an empty UDP packet with no time stamp.
 */
void LofarChunker::generateEmptyPacket(UDPPacket& packet)
{
    size_t size = sizeof(packet.data);
    memset((void*)packet.data, 0, size);
    packet.header.nrBlocks = 0;
    packet.header.timestamp = 0;
    packet.header.blockSequenceNumber = 0;
}

} // namespace pelican
