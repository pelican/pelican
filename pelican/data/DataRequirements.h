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

#ifndef DATAREQUIREMENTS_H
#define DATAREQUIREMENTS_H


/**
 * @file DataRequirements.h
 */

#include <QtCore/QSet>
#include <QtCore/QString>

namespace pelican {

/**
 * @class DataRequirements
 *
 * @brief
 * Specifies the types of data required by the pipeline modules
 * to operate a pipeline.
 *
 * @details
 *
 */

class DataRequirements
{
    public:
        DataRequirements();
        ~DataRequirements();

        /// add a data requirement that must be met
        void addRequired( const QString& stream );
        void addRequired( const QSet<QString>& stream );

        /// add a data requirement that would be nice
        //  to have, but is not essential
        void addOptional( const QString& stream );
        void addOptional( const QSet<QString>& stream );

        /// Returns a set containing all required data types.
        QSet<QString> allStreams() const;

        /// Return true if the specified stream is included
        bool contains(const QString& stream) const;

        /// Clears the data requirements object.
        void clear();

        /// Returns the set of required streams.
        inline const QSet<QString>& required() const {return _requiredData;}

        /// Returns the set of optional streams.
        inline const QSet<QString>& optional() const {return _optionalData;}

        /// return the number of streams in the object
        int size() const;

        /// Compute a hash value for use with QHash.
        uint hash() const;

        /// Test for equality with another object.
        bool operator==(const DataRequirements&) const;

        /// Test for inequality with another object.
        bool operator!=(const DataRequirements&) const;

        /// Merge another requirements object with this one.
        DataRequirements& operator+=(const DataRequirements&);

        /// substract the streams in the list and return the remainder
        void remove( const QList<QString>& list );

    private:
        mutable uint _hash;
        QSet<QString> _requiredData;
        QSet<QString> _optionalData;
};

/// Compute a hash value for use with QHash (uses the hash member function).
uint qHash(const DataRequirements& key);

bool operator==(const DataRequirements&, const QList<QString>&);

} // namespace pelican
#endif // DATAREQUIREMENTS_H
