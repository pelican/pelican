#ifndef LOFARCHUNKER_H
#define LOFARCHUNKER_H
#include <QString>
#include <QObject>
#include "server/LofarTypes.h"
#include "LofarUdpHeader.h"
#include "server/AbstractChunker.h"

/**
 * @file LofarChunker.h
 */

namespace pelican {

class DataManager;

/**
 * @class LofarChunker
 *
 * @brief
 * Implemnetation of an AbstractChunker to monitor calling.
 *
 * @details
 * 
 */
class LofarChunker : public AbstractChunker
{
        friend class LofarChunkerTest;

    public:
        /// Constructs a new LofarChunker.
        LofarChunker(const QString& type = "ChunkType",
                QString host = "", quint16 port = 0);

        /// Destroys the LofarChunker.
        ~LofarChunker() {}

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        ///
        virtual void next(QIODevice*);

        /// Sets the number of packets to read.
        void setPackets(int packets) {_nPackets = packets;}

    private:
        /// Generates an empty UDP packet.
        void generateEmptyPacket(UDPPacket& packet);

    private:
        int _nPackets;
        unsigned _packetsRejected;
        unsigned _packetsAccepted;
        unsigned _samplesPerPacket;
        TYPES::TimeStamp _startTime;
};

} // namespace pelican

#endif // LOFARCHUNKER_H
