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

#ifndef ABSTRACTDATABUFFER_H
#define ABSTRACTDATABUFFER_H

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QString>

/**
 * @file AbstractDataBuffer.h
 */

namespace pelican {

class WritableData;

/**
 * @ingroup c_server
 *
 * @class AbstractDataBuffer
 *
 * @brief
 * Interface class for generic data buffers.
 *
 * @details
 * This class provides an abstract interface for accessing chunks of data
 * held in data buffers, and defines the getWritable() method for this purpose.
 *
 * The class is inherited by StreamDataBuffer and ServiceDataBuffer.
 */
class AbstractDataBuffer : public QObject
{
    Q_OBJECT

    public:
        /// Constructs a new abstract data buffer object.
        AbstractDataBuffer(const QString& type, QObject* parent = 0) :
            QObject(parent), _type(type), _verboseLevel(0) {}

        /// Destroys the data buffer object.
        virtual ~AbstractDataBuffer() {}

        /// Returns a WritableData object of the specified size from the buffer.
        virtual WritableData getWritable(size_t size) = 0;

        /// set verbose levels
        void setVerbosity(int level) { _verboseLevel = level; }
        void verbose(const QString& msg, int level = 1);

    protected:
        QMutex _mutex;
        QMutex _writeMutex;
        QString _type;
        int _verboseLevel;
};

} // namespace pelican

#endif // ABSTRACTDATABUFFER_H
