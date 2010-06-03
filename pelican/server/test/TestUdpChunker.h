#ifndef TESTUDPCHUNKER_H
#define TESTUDPCHUNKER_H

#include "pelican/server/AbstractChunker.h"

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QByteArray>

/**
 * @file TestUdpChunker.h
 */

namespace pelican {

/**
 * @class TestUdpChunker
 *
 * @brief
 * Implementation of an AbstractChunker to work with the telescope emulator.
 *
 * @details
 *
 */
class TestUdpChunker : public AbstractChunker
{
    public:
        /// Constructs a new TestUdpChunker.
         TestUdpChunker(const ConfigNode& config);

        /// Destroys the TestChunker.
        ~TestUdpChunker() {};

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        /// Gets the next chunk of data.
        virtual void next(QIODevice*);

    private:
        unsigned _nextCount;
        size_t _chunkSize;
        QByteArray _tempBuffer;
};

} // namespace pelican

#endif // TESTUDPCHUNKER_H
