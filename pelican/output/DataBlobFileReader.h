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

#ifndef DATABLOBFILEREADER_H
#define DATABLOBFILEREADER_H
#include <QtCore/QString>

/**
 * @file DataBlobFileReader.h
 */

namespace pelican {
class DataBlobFileType;
class DataBlob;

/**
 * @class DataBlobFileReader
 *
 * @brief
 *    Read a file in the DataBlobFile format
 * @details
 *    This class provides a convenient way to read in DataBlobFile
 *    output streams. It can handle both heterogeneous and homogeneous
 *    file types
 *    The general procedure is:
 *    open() the file
 *    find out the next blob stored, with nextBlob()
 *    pass a DataBlob of this type to readData();
 */
class DataBlobFileReader
{
    public:
        /// DataBlobFileReader constructor.
        DataBlobFileReader();

        /// open the specified file for reading
        void open(const QString& filename);

        /// return the next DataBlob type in the stream
        QString nextBlob() const;

        /// read in data to the provided datablob
        void readData( DataBlob* blob );

        /// DataBlobFileReader destructor.
        ~DataBlobFileReader();

    private:
        DataBlobFileType* _file;
};

} // namespace pelican

#endif // DATABLOBFILEREADER_H
