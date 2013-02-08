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

#ifndef DATABLOB_H
#define DATABLOB_H

/**
 * @file DataBlob.h
 */

#include "utility/FactoryRegistrar.h"

#include <QtCore/QString>
#include <QtCore/QSysInfo>
#include <complex>

class QIODevice;


/**
 * This macro is used to register the named data blob type.
 */
#define PELICAN_DECLARE_DATABLOB(type) PELICAN_DECLARE(pelican::DataBlob, type)

namespace pelican {

/**
 * @ingroup c_data
 *
 * @class DataBlob
 *
 * @brief
 * Base class for all Pelican data blobs.
 *
 * @details
 * This is the base class used for all Pelican data blobs required by
 * pipeline modules.
 */

class DataBlob
{
    public:
        /// Data blob constructor.
        PELICAN_CONSTRUCT_TYPES_EMPTY
        DataBlob(const QString& type);

        /// Data blob destructor.
        virtual ~DataBlob() {}

    public:
        /// Returns the type of DataBlob (should be the class name).
        const QString& type() const {return _type;}

        /// Set the version ID of the DataBlob.
        void setVersion(const QString& v) { _version = v; }

        /// Returns the version of the DataBlob.
        const QString& version() const { return _version; }

    public:
        /// Serialise the DataBlob into the QIODevice.
        virtual void serialise(QIODevice&) const;

        /// Returns the number of serialised bytes.
        virtual quint64 serialisedBytes() const;

        /// Deserialises the DataBlob from the QIODevice.
        virtual void deserialise(QIODevice&, QSysInfo::Endian endianness);

    private:
        QString _version;
        QString _type;
};

} // namespace pelican
#endif // DATABLOB_H
