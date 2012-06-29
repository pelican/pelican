#include "DataBlobChunker.h"


namespace pelican {

/**
 *@details DataBlobChunker 
 */
DataBlobChunker::DataBlobChunker()
    : AbstractChunker()
{
}

/**
 *@details
 */
DataBlobChunker::~DataBlobChunker()
{
}

QIODevice* DataBlobChunker::newDevice() {
}

void DataBlobChunker::next(QIODevice*) {
    
}

} // namespace pelican
