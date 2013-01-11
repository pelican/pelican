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

#ifndef TESTDATABLOBCLIENT_H
#define TESTDATABLOBCLIENT_H

/**
 * @file TestDataBlobClient.h
 */

#include "output/AbstractDataBlobClient.h"
#include <QtCore/QSet>

namespace pelican {
namespace test {

/**
 * @ingroup t_output
 *
 * @class TestDataBlobClient
 *
 * @brief
 * A dummy DatBlobClient for unit testing
 *
 * @details
 */

class TestDataBlobClient : public AbstractDataBlobClient
{
    Q_OBJECT

    public:
        /// TestDataBlobClient constructor.
        TestDataBlobClient();

        /// TestDataBlobClient destructor.
        ~TestDataBlobClient();

        /// emits signals and send Data
        void send(const Stream&);

        /// return the set of all streams that have been subscribed to
        const QSet<QString>& subscriptions() const;

        // interface requirements
        virtual QSet<QString> streams();
        virtual void subscribe( const QSet<QString>& streams );

    private:
        QSet<QString> _streams;
        QSet<QString> _subs;
};

} // namespace test
} // namespace pelican
#endif // TESTDATABLOBCLIENT_H
