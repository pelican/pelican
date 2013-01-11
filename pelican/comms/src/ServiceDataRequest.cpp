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

#include "comms/ServiceDataRequest.h"

namespace pelican {


// class ServiceDataRequest
ServiceDataRequest::ServiceDataRequest()
    : ServerRequest(ServerRequest::ServiceData)
{
}

ServiceDataRequest::~ServiceDataRequest()
{
}

void ServiceDataRequest::request(const QString& type, const QString& version)
{
    _dataRequested[type] = version;
}

QList<QString> ServiceDataRequest::types() const
{
    return _dataRequested.keys();
}

QString ServiceDataRequest::version(const QString& type) const
{
    return _dataRequested.value(type);
}

bool ServiceDataRequest::isEmpty() const
{
    return _dataRequested.isEmpty();
}

bool ServiceDataRequest::operator==(const ServiceDataRequest& req) const
{
    return _dataRequested == req._dataRequested;
}

bool ServiceDataRequest::operator==(const ServerRequest& request) const
{
    bool r = ServerRequest::operator==(request);
    if( r ) {
        r = operator==( static_cast<const ServiceDataRequest& >( request ) );
    }
    return r;
}

} // namespace pelican
