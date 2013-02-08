/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DATABLOBCHUNKER_H
#define DATABLOBCHUNKER_H


#include "AbstractChunker.h"

/**
 * @file DataBlobChunker.h
 */

namespace pelican {
class DataBlobChunkerClient;

/**
 * @class DataBlobChunker
 *
 * @brief
 *    A pass-through chunker for DataBlobs emitted from a
 *    Pelican Output Streaming server
 * @details
 *    Each chunk corresponds to a single DataBlob
 */

class DataBlobChunker : public AbstractChunker
{
        friend class DataBlobChunkerClient;

    public:
        DataBlobChunker(const ConfigNode& config);
        ~DataBlobChunker();

        virtual QIODevice* newDevice();
        virtual void next(QIODevice*) {};

        void setPort(quint16 port);
        quint16 port() const;
        void setHost(const QString& ipaddress);

    private:
        DataBlobChunkerClient* _client;

};
PELICAN_DECLARE_CHUNKER(DataBlobChunker)

} // namespace pelican
#endif // DATABLOBCHUNKER_H
