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

#ifndef SERVICEDATAREQUEST_H
#define SERVICEDATAREQUEST_H

/**
 * @file ServiceDataRequest.h
 */

#include <QtCore/QHash>
#include "ServerRequest.h"

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class ServiceDataRequest
 *
 * @brief
 * A request for a specific version of service data.
 *
 * @details
 */

class ServiceDataRequest : public ServerRequest
{
    public:
        ServiceDataRequest();
        ~ServiceDataRequest();

    public:
        /// Adds a request for the data object of the specified type and version.
        void request(const QString& type, const QString& version);

        /// Return a list of data types in the request.
        QList<QString> types() const;

        /// Return the version of a specific type n.b only one version per type
        /// allowed per request.
        QString version(const QString& type) const;

        /// Returns true if n valid requests() calls have been made.
        bool isEmpty() const;

        /// Test for equality between ServiceData objects.
        bool operator==(const ServiceDataRequest&) const;

        virtual bool operator==(const ServerRequest&) const;

    private:
        QHash<QString,QString> _dataRequested;
};

} // namespace pelican
#endif // SERVICEDATAREQUEST_H
