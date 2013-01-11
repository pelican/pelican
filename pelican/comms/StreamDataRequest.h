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

#ifndef STREAMDATAREQUEST_H
#define STREAMDATAREQUEST_H

/**
 * @file StreamDataRequest.h
 */

#include <QtCore/QVector>
#include "comms/ServerRequest.h"
#include "data/DataSpec.h"
#include <iostream>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class StreamDataRequest
 *
 * @brief
 * Specifications of a set of DataSpec.
 *
 * @details
 */

class StreamDataRequest : public ServerRequest
{
    private:
        QVector<DataSpec> _dataOptions;

    public:
        typedef QVector<DataSpec>::const_iterator DataSpecIterator;

    public:
        StreamDataRequest();
        ~StreamDataRequest();

        /// Adds a dataset specification that is required.
        /// The first added option will be serviced if available
        /// otherwise the server will try to satisfy subsequent options
        void addDataOption(const DataSpec& data);

        /// Returns an iterator for the DataSpec options.
        /// The order will correspond to the addDataOption call sequence.
        DataSpecIterator begin() const {return _dataOptions.begin();}

        /// Returns an iterator at the end of the DataSpec options.
        DataSpecIterator end() const {return _dataOptions.end();}

        /// Returns true if there are no stream data
        /// in any of the requirements passed.
        bool isEmpty() const { return _dataOptions.empty(); }

        /// The number of requirements.
        int size() const {return _dataOptions.size();}

        /// Test for equality between ServiceData objects.
        virtual bool operator==(const ServerRequest&) const;
};

typedef StreamDataRequest::DataSpecIterator DataSpecIterator;

} // namespace pelican
#endif // STREAMDATAREQUEST_H
