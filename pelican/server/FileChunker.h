#ifndef FILECHUNKER_H
#define FILECHUNKER_H


#include "pelican/server/AbstractChunker.h"

/**
 * @file FileChunker.h
 */

namespace pelican {

/**
 * @ingroup c_server
 *
 * @class FileChunker
 *
 * @brief
 *   Chunker wrapper around a file
 * @details
 *   Provides a chunker that represents the contents of a file
 *   When the file changes, a new chunk will be generated
 */
class FileChunker : public AbstractChunker
{
    public:
        /// FileChunker constructor.
        FileChunker(const ConfigNode& config);

        /// FileChunker destructor.
        ~FileChunker();

        virtual QIODevice* newDevice();
        virtual void next(QIODevice*);

    private:
        QString _fileName;
};
PELICAN_DECLARE_CHUNKER(FileChunker)

} // namespace pelican

#endif // FILECHUNKER_H
