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

#ifndef PELICANPROTOCOL_H
#define PELICANPROTOCOL_H

/**
 * @file PelicanProtocol.h
 */

#include "AbstractProtocol.h"

namespace pelican {

class DataBlob;

/**
 * @ingroup c_comms
 *
 * @class PelicanProtocol
 *
 * @brief
 * The primary protocol for communication between pipelines and the server.
 *
 * @details
 */

class PelicanProtocol : public AbstractProtocol
{
    public:
        PelicanProtocol();
        ~PelicanProtocol();

    public:
        /// Construct a server request object from reading the specified socket
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket);

        /// Sends a list of supported stream and service data.
        virtual void send(QIODevice& device, const DataSupportResponse&);

        /// Send one or more stream data chunks with header information
        /// containing a description of associated service data.
        virtual void send(QIODevice& stream, const AbstractProtocol::StreamData_t&);

        /// Send a serialised data blob.
        virtual void send(QIODevice& stream, const QString& name, const DataBlob&);

        /// Send one or more service data chunks.
        virtual void send(QIODevice& stream, const AbstractProtocol::ServiceData_t&);

        /// Send a message string.
        virtual void send(QIODevice& stream, const QString&);

        /// Send a error.
        virtual void sendError(QIODevice& stream, const QString&);
};

} // namespace pelican
#endif // PELICANPROTOCOL_H
