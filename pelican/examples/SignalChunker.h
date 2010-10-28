#ifndef SIGNALCHUNKER_H
#define SIGNALCHUNKER_H

#include "pelican/server/AbstractChunker.h"

using namespace pelican;

/*
 * A simple example to demonstrate how to write a data chunker.
 */
class SignalChunker : public AbstractChunker
{
    public:
        SignalChunker(const ConfigNode& config);
        virtual QIODevice* newDevice();
        virtual void next(QIODevice*);

    private:
        qint64 _chunkSize;
        qint64 _bytesRead;
};

// Register the chunker.
PELICAN_DECLARE_CHUNKER(SignalChunker)

#endif // SIGNALCHUNKER_H
