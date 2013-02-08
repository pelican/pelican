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

#ifndef TYPECOUNTER_H
#define TYPECOUNTER_H

#include <QtCore/QList>

/**
 * @file TypeCounter.h
 */

namespace pelican {

/**
 * @class TypeCounter
 *
 * @brief
 *     maintains containers of values associated with a
 *     specific type. Returns the max, min values as
 *     the contributors to the type are dynamically added
 *     or removed
 * @details
 *     Uses the inserter sort algoritm, so that computation
 *     overhead is done only when adding() data
 */

template<typename T>
class TypeCounter
{
    public:
        TypeCounter() {};
        ~TypeCounter() {};

        // return true if there is no data
        bool isEmpty() const { return _data.isEmpty(); }

        // remove a value from the data
        void remove(const T& d) { _data.removeOne(d); }

        // add a value to the data
        void add(const T& t) {
            // Inserter directly into the correct place
            int j=0;
            while( j < _data.size() && _data[j] < t) {
                ++j;
            }
            _data.insert(j, t);
        }

        // return the maximum value. Must check with isEmpty() first
        const T& max() const { return _data.last(); }
        // return the minimum value. Must check with isEmpty() first
        const T& min() const { return _data.first(); }

    private:
        QList<T> _data;
};

} // namespace pelican
#endif // TYPECOUNTER_H
