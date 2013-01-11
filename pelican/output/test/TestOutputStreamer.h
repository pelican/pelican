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

#ifndef TESTOUTPUTSTREAMER_H
#define TESTOUTPUTSTREAMER_H

/**
 * @file TestOutputStreamer.h
 */

#include "output/AbstractOutputStream.h"
#include "utility/ConfigNode.h"
#include <QtCore/QString>
#include <QtCore/QMap>

namespace pelican {
class DataBlob;

namespace test {

/**
 * @ingroup t_output
 *
 * @class TestOutputStreamer
 *
 * @brief
 * An implementation of AbstractOutputStream for testing
 *
 * @details
 */

class TestOutputStreamer : public AbstractOutputStream
{
    public:
        TestOutputStreamer( const QString& id="TestOutputStreamer",
                const ConfigNode& config = ConfigNode());
        TestOutputStreamer( const ConfigNode& config );
        ~TestOutputStreamer();
        const QString& id() const { return _id; };
        const DataBlob* lastReceived( const QString& streamName );

    protected:
        virtual void sendStream(const QString& streamName, const DataBlob* dataBlob);

    private:
        QString _id;
        ConfigNode _config;
        QMap<QString, const DataBlob* > _received;
};

PELICAN_DECLARE(AbstractOutputStream, TestOutputStreamer)

} // namespace test
} // namespace pelican
#endif // TESTOUTPUTSTREAMER_H
