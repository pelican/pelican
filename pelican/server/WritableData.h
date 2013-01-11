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

#ifndef WRITABLEDATA_H
#define WRITABLEDATA_H

/**
 * @file WritableData.h
 */

#include "server/AbstractLockableData.h"

namespace pelican {

/**
 * @ingroup c_server
 *
 * @class WritableData
 *
 * @brief
 * Provides locked access to an area of memory to enable writing.
 *
 * @details
 * The data will become unlocked on destruction
 * and emit a signal when there are no locks upon it
 */
class WritableData
{
    public:
        WritableData() : _data(0) {}

        WritableData(AbstractLockableData* d);

        ~WritableData();

        void write(const void* dataBuffer, size_t size, size_t offset = 0);

        AbstractLockableData* data() const {return _data;}

        /// Returns a pointer to the start of the memory block.
        void* ptr() {return _data->data()->data();}

        /// returns true if there is a valid Data object
        bool isValid() const {return _data ? _data->isValid() : false;}

        WritableData& operator=(const WritableData& other);

    private:
        AbstractLockableData* _data;
};

} // namespace pelican

#endif // WRITABLEDATA_H
