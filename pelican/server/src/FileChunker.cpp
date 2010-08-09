#include "pelican/server/FileChunker.h"
#include "pelican/utility/WatchedFile.h"


#include "pelican/utility/memCheck.h"

namespace pelican {


/**
 * @details Constructs a FileChunker object.
 */
FileChunker::FileChunker(const ConfigNode& config)
    : AbstractChunker(config)
{
}

/**
 * @details Destroys the FileChunker object.
 */
FileChunker::~FileChunker()
{
}

QIODevice* FileChunker::newDevice()
{
    WatchedFile* device = new WatchedFile();
    device->open(QIODevice::ReadOnly);
    return device;
}

void FileChunker::next(QIODevice* dev)
{
    QFile* file=static_cast<QFile*>(dev);

    // seek to the beginning
    file->reset();

    // Get writable buffer space for the chunk.
    WritableData writableData = getDataStorage( file->size() );
    if( writableData.isValid() ) {
        // Get pointer to start of writable memory.
        char* ptr = (char*) (writableData.ptr());
        file->read( ptr, file->size() );
    }

}
PELICAN_DECLARE(AbstractChunker, FileChunker)

} // namespace pelican
