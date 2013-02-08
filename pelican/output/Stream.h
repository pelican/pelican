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

#ifndef STREAM_H
#define STREAM_H

/**
 * @file Stream.h
 */

#include <boost/shared_ptr.hpp>
#include <QtCore/QString>

namespace pelican {

class DataBlob;

/**
 * @ingroup c_output
 *
 * @class Stream
 *
 * @brief
 *   Contains info about a stream
 * @details
 *
 */
class Stream
{
    public:
        /// Stream constructor.
        Stream(const QString& streamName = QString() );

        /// return the name of the stream
        const QString& name() const { return _name; }

        /// return the data type of the stream
        const QString& type() const;

        /// return a shared ptr to the data
        boost::shared_ptr<DataBlob> data() const;

        /// set the data
        void setData( const boost::shared_ptr<DataBlob>& );

        /// Stream destructor.
        ~Stream();

    private:
        QString _type;
        QString _name;
        boost::shared_ptr<DataBlob> _data;

};

} // namespace pelican

#endif // STREAM_H
