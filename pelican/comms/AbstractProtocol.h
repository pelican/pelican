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

#ifndef ABSTRACTPROTOCOL_H
#define ABSTRACTPROTOCOL_H

/**
 * @file AbstractProtocol.h
 */

#include <boost/shared_ptr.hpp>
#include <QtCore/QMap>

class QString;
class QIODevice;
class QTcpSocket;

namespace pelican {

class ServerRequest;
class StreamData;
class DataChunk;
class DataBlob;
class DataSupportResponse;

/**
 * @ingroup c_comms
 *
 * @class AbstractProtocol
 *
 * @brief
 * Base class for all server side protocol interpretations.
 *
 * @details
 * The protocol interprets the requests of a particular client type into
 * the ServerRequest Objects.
 * Conversely it converts the internal types to be returned into the
 * format expected by the client.
 */
class AbstractProtocol
{
    public:
       typedef QList<StreamData*> StreamData_t;
       typedef QList<DataChunk*> ServiceData_t;

    public:
        AbstractProtocol() {}
        virtual ~AbstractProtocol() {}

        /// Processes an incoming request.
        virtual boost::shared_ptr<ServerRequest> request(QTcpSocket& socket) = 0;

        /// Write stream data to an I/O device.
        virtual void send(QIODevice& device, const StreamData_t&) = 0;

        /// Write service data to an I/O device.
        virtual void send(QIODevice& device, const ServiceData_t&) = 0;

        /// Write out a DataBlob object to an I/O Device, as a stream of "name"
        virtual void send(QIODevice& device, const QString& name, const DataBlob& ) = 0;

        /// Write a non-error message to an I/O device.
        virtual void send(QIODevice& device, const QString& message) = 0;

        /// Write a non-error message to an I/O device.
        virtual void send(QIODevice& device, const DataSupportResponse& message) = 0;

        /// Send an error to an I/O device.
        virtual void sendError(QIODevice& device, const QString&) = 0;

};

} // namespace pelican

#endif // ABSTRACTPROTOCOL_H
