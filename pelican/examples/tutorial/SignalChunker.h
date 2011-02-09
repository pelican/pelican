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
        // Constructs the chunker.
        SignalChunker(const ConfigNode& config);

        // Creates the input device (usually a socket).
        virtual QIODevice* newDevice();

        // Obtains a chunk of data from the device when data is available.
        virtual void next(QIODevice*);

    private:
        qint64 _chunkSize;
        qint64 _bytesRead;
};

PELICAN_DECLARE_CHUNKER(SignalChunker)

#endif // SIGNALCHUNKER_H
