#ifndef TESTCHUNKER_H
#define TESTCHUNKER_H
#include <QString>
#include "server/AbstractChunker.h"

/**
 * @file TestChunker.h
 */

namespace pelican {
class DataManager;

/**
 * @class TestChunker
 *
 * @brief
 * Implemnetation of an AbstractChunker to monitor calling.
 *
 * @details
 * 
 */
class TestChunker : public AbstractChunker
{
    public:
        /// Constructs a new TestChunker.
        TestChunker(DataManager* dm, const QString& type = "ChunkType",
                bool badSocket = false, size_t size = 0, QString host = "",
                quint16 port = 0)
        : AbstractChunker(type, dm, host, port), _badSocket(badSocket),
        _nextCount(0), _size(size) {}

        /// Destroys the TestChunker.
        ~TestChunker() {}

        /// Creates the socket to use for the incoming data stream.
        virtual QIODevice* newDevice();

        ///
        virtual void next(QIODevice*);

        /// return the number of times that the next method has been called
        //  will also reset the counter
        unsigned int nextCalled();

    private:
        bool _badSocket;
        unsigned int _nextCount;
        size_t _size;
};

} // namespace pelican

#endif // TESTCHUNKER_H 
