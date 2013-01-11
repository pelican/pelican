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

#ifndef STREAMDATA_H
#define STREAMDATA_H

/**
 * @file StreamData.h
 */

#include "comms/DataChunk.h"

#include <QtCore/QSet>
#include <QtCore/QString>
#include <boost/shared_ptr.hpp>

namespace pelican {

class LockedData;

/**
 * @ingroup c_comms
 *
 * @class StreamData
 *
 * @brief
 *     Contains Pointers to Chunked Stream and a manifest of associated data
 * @details
 *     As well as a pointer to and the size of the data this class
 *     also contains linking information to the service data
 */

class StreamData : public DataChunk
{
    public:
        typedef QList<boost::shared_ptr<DataChunk> > DataList_t;

    public:
        StreamData(const QString& name, void* = 0, size_t  = 0);
        StreamData(const QString& name, QString& id, size_t  = 0);
        StreamData(const QString& name, QString& id, QByteArray& d);
        virtual ~StreamData();

        void addAssociatedData(boost::shared_ptr<DataChunk>);
        const DataList_t& associateData() const;
        const QSet<QString>& associateDataTypes() const;
        virtual bool isValid() const;
        bool operator==(const StreamData& sd) const;
        void reset( size_t );

    private:
        StreamData(const StreamData&);

    private:
        DataList_t _associateData;
        QSet<QString> _associateDataTypes;
};

} // namespace pelican
#endif // STREAMDATA_H
