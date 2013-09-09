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

#ifndef ABSTRACTLOCKABLEDATA_H
#define ABSTRACTLOCKABLEDATA_H

/**
 * @file AbstractLockableData.h
 */

#include <boost/shared_ptr.hpp>
#include "server/AbstractLockable.h"
#include "comms/DataChunk.h"

namespace pelican {

class DataChunk;

/**
 * @ingroup c_server
 *
 * @class AbstractLockableData
 *
 * @brief
 * Interface for data classes that can be locked.
 *
 * @details
 * \todo Detailed description for AbstractLockableData.
 */
class AbstractLockableData : public AbstractLockable
{
    Q_OBJECT

    protected:
        boost::shared_ptr<DataChunk> _data;

    private:
        quint64 _maxSize;

    public:
        /// Constructs a new AbstractLockableData object.
        AbstractLockableData( quint64 maxSize, QObject* parent )
        : AbstractLockable(parent), _maxSize(maxSize) {}

        /// Destroys the AbstractLockableData object.
        virtual ~AbstractLockableData() {}

        /// Return the data ID.
        QString id() const { return _data->id(); }

        /// Set the object identity.
        void setId(const QString& id) { _data->setId(id); }

        /// Return the underlying data object.
        boost::shared_ptr<DataChunk> data() const { return _data; }

        /// Return the maximum size.
        quint64 maxSize() const { return _maxSize; }
};

} // namespace pelican

#endif // ABSTRACTLOCKABLEDATA_H
