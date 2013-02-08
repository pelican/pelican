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

#ifndef LOCKABLESERVICEDATA_H
#define LOCKABLESERVICEDATA_H

#include <cstring>
#include "server/AbstractLockableData.h"

/**
 * @file LockableServiceData.h
 */

namespace pelican {

class DataChunk;

/**
 * @ingroup c_server
 *
 * @class LockableServiceData
 *
 * @brief
 * Primary interface to access chunks of data in the server.
 *
 * @details
 * This class takes care of locking etc.
 */
class LockableServiceData : public AbstractLockableData
{
    Q_OBJECT

    public:
        /// Constructs a new LockableServiceData object.
        LockableServiceData(const QString& name = "", void* data = 0,
                size_t size = 0, QObject* parent = 0);

        /// Destroys the LockableServiceData object.
        virtual ~LockableServiceData() {}

        /// Return the size of the stored data.
        size_t size() const {return _data->size();}

        /// set the size of data (must be less than the maxSize)
        void setSize(size_t s ) { _data->setSize(s); }

        /// Returns a pointer to the beginning of the memory block.
        void* memoryLocation() {return _data->ptr();}

        /// Return the name of the data.
        QString name() const {return _data->name();}

        /// Returns true if there is any valid data in the object.
        virtual bool isValid() const {return _data->isValid();}

    private:
        /// Disallow the copy constructor.
        LockableServiceData(const LockableServiceData&);
};

} // namespace pelican

#endif // LOCKABLESERVICEDATA_H
