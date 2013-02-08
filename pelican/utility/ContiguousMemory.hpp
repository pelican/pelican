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

#ifndef CONTIGUOUSMEMORY_H
#define CONTIGUOUSMEMORY_H
#include <memory>
#include <QtCore/QList>
#include <QtCore/QString>

/**
 * @file ContiguousMemory.h
 */

namespace pelican {

/**
 * @class ContiguousMemory
 *
 * @brief
 *    A C++ interface to the std::get_temporary_buffer
 * @details
 *
 */

template<typename T>
class ContiguousMemory
{
    private:
        T* _buf;
        QList<unsigned int> _free;
        unsigned long _size;

    public:
        // reserve a contiguous memory buffer for the specified number
        // of objects
        ContiguousMemory( unsigned long num ) : _size(num) {
            // allocate the memory
            std::pair<T*,unsigned long> r = std::get_temporary_buffer<T>(num);
            if( r.second != num ) throw(QString("ContiguousMemory: unable to allocate num objects"));
            _buf = r.first;
            // mark all slots as free
            for(unsigned int i=0; i<_size; ++i ) {
                _free.append(i);
            }
        };
        ~ContiguousMemory() { std::return_temporary_buffer<T>(_buf); }

        // get a pointer to a free memory slot - or NULL if no memory available
        T* nextFree() {
           if(_free.size() == 0) return NULL;
           return &_buf[_free.takeFirst()];
        }

        /// recover memory in the buffer for the specified object
        void free( T* object ) {
            Q_ASSERT( object >= _buf && object <= _buf+(_size*sizeof(T)));
            _free.append( object - _buf );
        }

};

} // namespace pelican
#endif // CONTIGUOUSMEMORY_H
