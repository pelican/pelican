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

#ifndef SERVERREQUEST_H
#define SERVERREQUEST_H

/**
 * @file ServerRequest.h
 */

#include <QtCore/QString>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class ServerRequest
 *
 * @brief
 * Encapsulates the data requests to the server.
 *
 * @details
 * Requests must have a type and ancillary data.
 */

class ServerRequest
{
    public:
        typedef enum {
            Error, Acknowledge, StreamData, ServiceData, DataSupport
        } Request;

    private:
        Request _type;
        QString _error;

    public:
        /// Constructs a new ServerRequest object.
        ServerRequest(Request type = Error, const QString& msg = "")
        : _type(type), _error(msg) {}

        /// Destroys the ServerRequest object.
        virtual ~ServerRequest() {}

        /// Returns the request type.
        Request type() const {return _type;}

        /// Sets the error message.
        void error(const QString& msg) {_error = msg; _type = Error;}

        /// Returns the error message.
        const QString& message() const {return _error;}

        /// Tests whether this ServerRequest type is the same as another.
        virtual bool operator==(const ServerRequest& req) const
        { return _type == req._type; }
};

} // namespace pelican

#endif // SERVERREQUEST_H
