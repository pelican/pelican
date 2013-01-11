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

#ifndef DATASUPPORTRESPONSE_H
#define DATASUPPORTRESPONSE_H

/**
 * @file DataSupportResponse.h
 */

#include "ServerResponse.h"
#include "data/DataSpec.h"

#include <QtCore/QSet>
#include <QtCore/QString>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class DataSupportResponse
 *
 * @brief
 * Contains a list of supported streams.
 *
 * @details
 */

class DataSupportResponse : public ServerResponse
{
    public:
        DataSupportResponse(const DataSpec& spec );
        DataSupportResponse(const QSet<QString>& streams, const QSet<QString>& service = QSet<QString>() );
        ~DataSupportResponse();

        /// set the default adapter types for each stream
        void setDefaultAdapters( const QHash<QString, QString>& a ) { _spec.addAdapterTypes(a); }

        const QSet<QString>& streamData() const { return _spec.streamData(); }
        const QSet<QString>& serviceData() const { return _spec.serviceData(); }

        /// return a hash of Adapter types for each stream to use as a default
        //  if not overidden elsewhere
        const QHash<QString, QString>& defaultAdapters() const { return _spec.getAdapterTypes(); }

    private:
         DataSpec _spec;
};

} // namespace pelican
#endif // DATASUPPORTRESPONSE_H
