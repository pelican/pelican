#include "ChunkerTester.h"
#include "pelican/server/StreamDataBuffer.h"


#include "pelican/utility/memCheck.h"

namespace pelican {

/**
 * @details Constructs a ChunkerTester object.
 */
ChunkerTester::ChunkerTester( AbstractChunker& chunker)
    : _chunker(&chunker)
{
    run();
}

/**
 * @details Destroys the ChunkerTester object.
 */
ChunkerTester::~ChunkerTester()
{
}

/**
 * @details start the chunker listening
 */
void ChunkerTester::run()
{
}

int ChunkerTester::writeRequestCount() const
{
    return _buffer->numberOfActiveChunks();
}

} // namespace pelican
