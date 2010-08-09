#ifndef CHUNKERTESTER_H
#define CHUNKERTESTER_H


/**
 * @file ChunkerTester.h
 */

namespace pelican {
    class AbstractChunker;
    class StreamDataBuffer;
    class LockableData;

/**
 * @class ChunkerTester
 * 
 * @brief
 *   Convenience class for testing a chunker
 * @details
 * 
 */
class ChunkerTester
{
    public:
        /// ChunkerTester constructor.
        ChunkerTester( AbstractChunker& chunker );

        /// ChunkerTester destructor.
        ~ChunkerTester();

        /// returns the number of times getWritable was called be the chunker
        int writeRequestCount() const;

        /// start the chunker listening
        void run();

        /// return the data resulting from the last run() call
        LockableData getData();

    private:
        StreamDataBuffer* _buffer;
        AbstractChunker* _chunker;
};

} // namespace pelican

#endif // CHUNKERTESTER_H 
