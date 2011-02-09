#include "reference/DirectDataClientExample.h"
#include "reference/ChunkerExample.h"


using namespace pelican;

/**
 * @details Constructs a DirectDataClientExample object.
 */
DirectDataClientExample::DirectDataClientExample(const ConfigNode& configNode,
                const DataTypes& types, const Config* config)
    : DirectStreamDataClient(configNode, types, config)
{
    addChunker("Stream1", "ChunkerExample", "Stream1ConfigName");
    addChunker("Stream2", "ChunkerExample", "Stream2ConfigName");
}

/**
 * @details Destroys the DirectDataClientExample object.
 */
DirectDataClientExample::~DirectDataClientExample()
{
}
