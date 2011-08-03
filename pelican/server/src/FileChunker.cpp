#include "pelican/server/FileChunker.h"
#include "pelican/utility/WatchedFile.h"

namespace pelican {


/**
 * @details Constructs a FileChunker object.
 */
FileChunker::FileChunker(const ConfigNode& config)
    : AbstractChunker( config)
{
    if( config.getDomElement().isNull() )
    {
        throw( QString("FileChunker: empty config passed in constructor") );
    }
    if( ! config.hasAttribute("file") )
        throw( QString("FileChunker: no \"file\" attribute specified" ));
    _fileName = config.getAttribute("file");
    if( _fileName == "" )
        throw( QString("FileChunker: no filename specified" ));
}

/**
 * @details Destroys the FileChunker object.
 */
FileChunker::~FileChunker()
{
}

QIODevice* FileChunker::newDevice()
{
    WatchedFile* device = new WatchedFile(_fileName);
    if( ! device->open(QIODevice::ReadOnly) )
        throw(QString("FileChunker: unable to open file:%1").arg(_fileName));
    //std::cout << "Watching file " << device->fileName().toStdString() << std::endl;
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
