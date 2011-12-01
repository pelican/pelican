#include "DirectoryWatchChunker.h"
#include "pelican/utility/WatchedDir.h"


namespace pelican {


/**
 *@details DirectoryWatchChunker 
 */
DirectoryWatchChunker::DirectoryWatchChunker( const ConfigNode& config )
    : AbstractChunker(config)
{
    if( config.getDomElement().isNull() )
    {
        throw( QString("DirectoryWatchChunker: empty config passed in constructor") );
    }
    if( ! config.hasAttribute("dir") )
        throw( QString("DirectoryWatchChunker: no \"dir\" attribute specified" ));
    _dirName = config.getAttribute("dir");
    if( _dirName == "" )
        throw( QString("DirectoryWatchChunker: no dirname specified" ));
}

/**
 *@details
 */
DirectoryWatchChunker::~DirectoryWatchChunker()
{
}

QIODevice* DirectoryWatchChunker::newDevice() {
    WatchedDir* device = new WatchedDir(_dirName);
    if( ! device->open(QIODevice::ReadOnly) )
        throw(QString("DirectoryWatchChunker: unable to open file:%1").arg(_dirName));
    return device;
}

void DirectoryWatchChunker::next(QIODevice* dev) {
    // get the new file
    WatchedDir* file=static_cast<WatchedDir*>(dev);

    // Get writable buffer space for the chunk.
    WritableData writableData = getDataStorage( file->size() );
    if( writableData.isValid() ) {
        // Get pointer to start of writable memory.
        char* ptr = (char*) (writableData.ptr());
        file->readFile( ptr, file->size() );
    }
}
PELICAN_DECLARE(AbstractChunker, DirectoryWatchChunker )

} // namespace pelican
