#ifndef CHUNKERTESTER_H
#define CHUNKERTESTER_H

#include <QtCore/QList>
#include <QtCore/QString>
/**
 * @file ChunkerTester.h
 */

#include "pelican/utility/Config.h"


namespace pelican {

class AbstractChunker;
class DataManager;
class LockedData;
class ChunkerManager;
class Config;

namespace test {

/**
 * @ingroup t_server
 *
 * @class ChunkerTester
 *
 * @brief
 * Convenience class for testing a chunker.
 *
 * @details
 * The tester will construct and start your chunker listening
 * on the appropriate channel.
 * example:
 *
 *
 * QString chunkerConfig("<MyDataChunker>");
 * chunkerConfig += QString("<connect host=\"localhost\" port=\"%1\" \>"
 *                  "</MyDataChunker>").arg( port);
 * ChunkerTester t("MyDataChunker" , chunkSize*10, chunkerConfig );
 * QUdpSocket socket;
 * socket.write( testData, "localhost", port );
 *
 */

class ChunkerTester
{
    public:
        /// ChunkerTester constructor.
        ChunkerTester(const QString& chunkerType, unsigned long bufferSize,
                const QString& XMLconfig = QString(), int verbosity=0 );

        /// ChunkerTester destructor.
        ~ChunkerTester();

        /// returns the number of times getWritable was called be the chunker
        //  for the specified stream (or default stream ifnot specified)
        int writeRequestCount( const QString& stream = QString() ) const;

        /// return the data resulting from the last run() call
        LockedData getData( const QString& stream = QString() );

        /// return pointer to the chunker created in the tester
        AbstractChunker* chunker() const;

        /// return pointer to the current device associated with the chunker
        QIODevice* getCurrentDevice() const;

    private:
        QList<QString>   _streams; // name of the test stream
        ChunkerManager*  _chunkManager;
        DataManager*     _dataManager;
        Config           _config;
};

} // namespace test
} // namespace pelican
#endif // CHUNKERTESTER_H
