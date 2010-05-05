#ifndef CHUNKEREXAMPLE_H
#define CHUNKEREXAMPLE_H

#include "pelican/server/AbstractChunker.h"

/**
 * @file ChunkerExample.h
 */

using namespace pelican;

/**
 * @class ChunkerExample
 *  
 * @brief
 *   A simple example to demonstrate how to write a 
 *   Chunker
 */

class ChunkerExample : public AbstractChunker
{
    public:
        ChunkerExample( const ConfigNode& config );
        virtual ~ChunkerExample();
        virtual QIODevice* newDevice();
        virtual void next(QIODevice*);

    private:
        int packetSize;
};

#endif // CHUNKEREXAMPLE_H 
