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

#ifndef LOCKABLESTREAMDATA_H
#define LOCKABLESTREAMDATA_H

#include <QtCore/QSet>
#include <QtCore/QString>
#include "server/AbstractLockableData.h"

/**
 * @file LockableStreamData.h
 */

namespace pelican {

class StreamData;
class LockedData;

/**
 * @ingroup c_server
 *
 * @class LockableStreamData
 *
 * @brief
 * Contains pointers to chunked LockableStream and a manifest of
 * associated data.
 *
 * @details
 * As well as a pointer to and the size of the data this class
 * also contains linking information to the service data
 */
class LockableStreamData : public AbstractLockableData
{
    Q_OBJECT

    public:
        typedef QList<LockedData> DataList_t;

    public:
        /// LockableStreamData constructor.
        LockableStreamData(const QString& name, void* memory = 0,
                size_t = 0, QObject* parent = 0);

        /// LockableStreamData destructor.
        ~LockableStreamData();

        StreamData* streamData() const;

        /// Test validity of all data.
        bool isValid() const;

        /// Returns true if the object has been served.
        bool& served() { return _served; };

        /// Test validity of data only taking into account the named associates.
        bool isValid(const QSet<QString>&) const;

        /// Associates a specific dataset with this LockableStreamData chunk.
        void addAssociatedData(const LockedData&);

        /// Returns the datasets associated with this LockableStreamData chunk.
        const DataList_t& associateData() const { return _serviceData;}
        const QSet<QString>& associateDataTypes() const { return _serviceDataTypes;}

        /// Reset the object's data and mark it of a suitable size
        void reset( size_t size );

    private:
        /// Disable copy constructor.
        LockableStreamData(const LockableStreamData&);

    private:
        DataList_t _serviceData;
        QSet<QString> _serviceDataTypes;
        bool _served;
};

} // namespace pelican

#endif // LOCKABLESTREAMDATA_H
