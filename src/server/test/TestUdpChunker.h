#ifndef TESTUDPCHUNKER_H
#define TESTUDPCHUNKER_H

#include <QString>
#include <QObject>
#include <cstring>
#include <QByteArray>
#include "server/AbstractChunker.h"

/**
 * @file TestUdpChunker.h
 */

namespace pelican {

class DataManager;

/**
 * @class TestUdpChunker
 *
 * @brief
 * Implemnetation of an AbstractChunker to work with the Telescope emulator.
 *
 * @details
 * 
 */

class TestUdpChunker : public AbstractChunker
{
    public:
        /// Constructs a new TestUdpChunker.
        TestUdpChunker(const QString& type = "type", const QString host = "",
                       const quint16 port = 0, const size_t size = 0);

        /// Destroys the TestChunker.
        ~TestUdpChunker();

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        ///
        virtual void next(QIODevice*);

    private:
        unsigned int _nextCount;
        size_t _size;
        QByteArray _tempBuffer;
};

} // namespace pelican

#endif // TESTUDPCHUNKER_H 
