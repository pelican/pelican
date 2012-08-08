#ifndef DIRECTORYWATCHCHUNKER_H
#define DIRECTORYWATCHCHUNKER_H


#include "AbstractChunker.h"

/**
 * @file DirectoryWatchChunker.h
 */

namespace pelican {

/**
 * @class DirectoryWatchChunker
 *
 * @brief
 *    Monitors a directory for new files
 * @details
 *    Watches a Directory, As new files appear they are
 *    sent as a new chunk
 *
 */

class DirectoryWatchChunker : public AbstractChunker
{
    public:
        DirectoryWatchChunker( const ConfigNode& config );
        ~DirectoryWatchChunker();

        virtual QIODevice* newDevice();
        virtual void next(QIODevice*);

    private:
        QString _dirName;
};
PELICAN_DECLARE_CHUNKER(DirectoryWatchChunker)

} // namespace pelican
#endif // DIRECTORYWATCHCHUNKER_H 
