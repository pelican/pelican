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

#ifndef CIRCULARBUFFERITERATOR_H
#define CIRCULARBUFFERITERATOR_H

#include <QtCore/QList>

/**
 * @file CircularBuffer.h
 */

namespace pelican {

/**
 * @class CircularBuffer
 *
 * @brief
 *   A generic class for iterating over a QList
 *   in a circular fashion.
 * @details
 *
 */

template<typename T>
class CircularBufferIterator
{

    public:
        CircularBufferIterator( QList<T>* buffer )
                : _index(-1) { reset(buffer); };

        /// refresh to the new buffer
        /// The index will be moved to point to beginning
        void reset( QList<T>* buffer ) {
             _data = buffer;
             _size = _data->size();
             _index = _size - 1; // put index on last element
        }
        ~CircularBufferIterator() {};

        /// return the buffer
        QList<T>* buffer() { return _data; }

        /// reset the index to point to the position in the
        /// buffer containing the provided value
        /// If this value does not exist, the index will reset
        /// to the beginning of the buffer
        /// note that after this call, the operator* can become
        /// invalid until next() has been called.
        void resetIndex( const T& value ) {
            _index = _data->indexOf(value);
        }

        /// get the next item from the buffer
        T& next() {
            _index=(_index+1)%_size;
            return (*_data)[_index];
        };

        /// return the next item from the buffer, without moving the index
        const T& peekNext() {
            return (*_data)[(_index+1)%_size];
        }

        /// return the value currently pointed to
        //  undefined before next() has been called
        T& operator*() { return (*_data)[_index]; }

    private:
        QList<T>* _data;
        unsigned int _index;
        unsigned int _size;
};

} // namespace pelican
#endif // CIRCULARBUFFERITERATOR_H
