#ifndef DATABLOBCHUNKER_H
#define DATABLOBCHUNKER_H


#include "AbstractChunker.h"

/**
 * @file DataBlobChunker.h
 */

namespace pelican {

/**
 * @class DataBlobChunker
 *  
 * @brief
 *    A simple pass-through chunker for DataBlobs emittted from a Pelican Output Streaming server
 * @details
 * 
 */

class DataBlobChunker : public AbstractChunker
{
    public:
        DataBlobChunker();
        ~DataBlobChunker();

        virtual QIODevice* newDevice();
        virtual void next(QIODevice*);

    private:
        DataBlobClient _client;
};

} // namespace pelican
#endif // DATABLOBCHUNKER_H 
