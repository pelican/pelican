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

#ifndef THREADEDDATABLOBCLIENT_H
#define THREADEDDATABLOBCLIENT_H

#include <QtCore/QThread>
#include "AbstractDataBlobClient.h"
#include "utility/ConfigNode.h"

/**
 * @file ThreadedDataBlobClient.h
 */

namespace pelican {
    class ThreadedClientImpl;

/**
 * @ingroup c_output
 *
 * @class ThreadedDataBlobClient
 *
 * @brief
 *    A DataBlobClient that operates in its own thread
 * @details
 *
 */
class ThreadedDataBlobClient : public AbstractDataBlobClient
{
    Q_OBJECT

    public:
        /// ThreadedDataBlobClient constructor.
        ThreadedDataBlobClient(const ConfigNode& c);

        /// ThreadedDataBlobClient destructor.
        ~ThreadedDataBlobClient();

        /// set the host to listen to
        void setHost(const QString& host);

        /// set the port to listen on
        void setPort(quint16 port);

        // base class interface requirements
        virtual QSet<QString> streams();
        virtual void subscribe( const QSet<QString>& streams );

    protected slots:
        void emitNewStreamsAvailable();
        void emitNewData(const Stream&);

    private:
        ThreadedClientImpl* _thread;
        ConfigNode _config;
};

} // namespace pelican

#endif // THREADEDDATABLOBCLIENT_H
