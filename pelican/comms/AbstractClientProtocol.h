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

#ifndef ABSTRACTCLIENTPROTOCOL_H
#define ABSTRACTCLIENTPROTOCOL_H

#include <boost/shared_ptr.hpp>
class QByteArray;
class QAbstractSocket;

/**
 * @file AbstractClientProtocol.h
 */

namespace pelican {

class ServerRequest;
class ServerResponse;
class DataBlob;

/**
 * @ingroup c_comms
 *
 * @class AbstractClientProtocol
 *
 * @brief
 * Base class for all protocols on the client side.
 *
 * @details
 *
 */
class AbstractClientProtocol
{
    public:
        /// Creates a new AbstractClientProtocol.
        AbstractClientProtocol();

        /// Destroys the AbstractClientProtocol (virtual).
        virtual ~AbstractClientProtocol();

        /// Serialises the ServerRequest (pure virtual).
        virtual QByteArray serialise(const ServerRequest&) = 0;

        /// Translate incomming bit stream from a socket into
        //  appropriate ServerResponse objects
        virtual boost::shared_ptr<ServerResponse> receive(QAbstractSocket&) = 0;

};

} // namespace pelican

#endif // ABSTRACTCLIENTPROTOCOL_H
