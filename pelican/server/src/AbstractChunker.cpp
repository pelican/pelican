#include "server/AbstractChunker.h"
#include "utility/ConfigNode.h"

#include <iostream>

namespace pelican {

/**
 * @details
 *  Setup options in the configuration file:
 *    listen to an incoming port
 *    <connection host="dataHost" port="12345" />
 *    To set the stream name for the chunker
 *    <data type="streamName" />
 *    To set a default adapter for the specifc stream
 *    <data type="streamName" adapter="AdapterType" />
 */
AbstractChunker::AbstractChunker(const ConfigNode& config)
{
    // Initialise members.
    _dataManager = 0;

    _chunkTypes = config.getOptionList("data", "type");
    _adapterTypes = config.getOptionHash("data","type","adapter");
    _host = config.getOption("connection", "host", "");
    _port = (quint16)config.getOption("connection", "port", "0").toUInt();

    _active = true;
}


/**
 * @details
 * Destroys the AbstractChunker.
 */
AbstractChunker::~AbstractChunker()
{
    stop();
}

void AbstractChunker::setDataManager(DataManager* dataManager)
{ 
    _dataManager = dataManager; 
    dataManager->addDefaultAdapters( defaultAdapters() );
}

/**
 * @details
 * Returns a writable data object of the specified \p size.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
 *
 * @param[in] size  The size of the chunk requested on the buffer.
 */
WritableData AbstractChunker::getDataStorage(size_t size) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");

    if (_chunkTypes.size() != 1)
        throw QString("AbstractChunker::getDataStorage(): "
                "More than one chunk type registered, ambiguous request.");

    return _dataManager->getWritableData(_chunkTypes[0], size);
}


/**
 * @details
 * Returns a writable data object of the specified \p size and \p type.
 *
 * This method should be used by the chunker to access memory in a stream
 * or service FIFO buffer to ensure that the chunker only ever writes into an
 * available, locked location.
 *
 * @param[in] size  The size of the chunk requested on the buffer.
 * @param[in] type  The type (name) of the chunk in the data buffer.
 */
WritableData AbstractChunker::getDataStorage(size_t size,
        const QString& chunkType) const
{
    if (!_dataManager)
        throw QString("AbstractChunker::getDataStorage(): No data manager.");

    return _dataManager->getWritableData(chunkType, size);
}

void AbstractChunker::setDefaultAdapter( const QString& adapter ) {
    if (_chunkTypes.size() != 1)
        throw QString("AbstractChunker::setDefaultAdapter(): "
                "More than one chunk type registered, ambiguous request.");
   setDefaultAdapter( adapter, _chunkTypes[0] );
}

void AbstractChunker::setDefaultAdapter( const QString& adapter, 
                                const QString& stream )
{
    _adapterTypes.insert(stream,adapter);
    
}

} // namespace pelican
