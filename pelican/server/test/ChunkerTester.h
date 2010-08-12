#ifndef CHUNKERTESTER_H
#define CHUNKERTESTER_H

#include "pelican/utility/Config.h"

class QString;

/**
 * @file ChunkerTester.h
 */

namespace pelican {
    class AbstractChunker;
    class DataManager;
    class LockedData;
    class ChunkerManager;
    class Config;

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
        ChunkerTester( const QString& chunkerType, unsigned long bufferSize, const QString& XMLconfig = QString() );

        /// ChunkerTester destructor.
        ~ChunkerTester();

        /// returns the number of times getWritable was called be the chunker
        int writeRequestCount() const;

        /// return the data resulting from the last run() call
        LockedData getData();

        /// return pointer to the chunker created in the tester
        AbstractChunker* chunker();

    private:
        QString            _stream; // name of the test stream
        ChunkerManager*    _chunkManager;
        DataManager*       _dataManager;
        Config             _config;
};

} // namespace pelican

#endif // CHUNKERTESTER_H 
