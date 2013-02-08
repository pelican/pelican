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

#include "data/DataBlob.h"
#include <QtCore/QIODevice>

namespace pelican {

/**
 * @details
 * Constructs a new data blob, setting the type-name.
 *
 * @param[in] type The name of the data blob derived class.
 */
DataBlob::DataBlob(const QString& type) : _type(type)
{
}

/**
 * @details
 * Serialises the DataBlob into the QIODevice.
 * This method should be re-implemented in a derived class if needed,
 * since the default implementation will throw an exception of type QString.
 */
void DataBlob::serialise(QIODevice&) const
{
    throw QString("DataBlob: This object's ('%1') serialise method "
            "is undefined.").arg(type());
}

/**
 * @details
 * This method must return the number of bytes saved to the QIODevice
 * for the serialised data blob when calling the serialise() method.
 */
quint64 DataBlob::serialisedBytes() const
{
    throw QString("DataBlob: This object's ('%1') serialisedBytes method "
            "is undefined.").arg(type());
}

/**
 * @details
 * Deserialises the data blob.
 * This method should be re-implemented in a derived class if needed,
 * since the default implementation will throw an exception of type QString.
 */
void DataBlob::deserialise(QIODevice&, QSysInfo::Endian)
{
    throw QString("DataBlob: This object's ('%1') deserialise method "
            "is undefined.").arg(type());
}

} // namespace pelican
