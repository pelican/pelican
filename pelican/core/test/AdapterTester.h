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

#ifndef ADAPTERTESTER_H
#define ADAPTERTESTER_H

#include <QtCore/QString>
#include <QtCore/QHash>
#include "utility/FactoryConfig.h"
#include "utility/Config.h"
#include "core/AbstractAdapter.h"

class QIODevice;

/**
 * @file AdapterTester.h
 */

namespace pelican {
class DataBlob;
namespace test {


/**
 * @class AdapterTester
 *
 * @brief
 *     Helper for writing Unit tests for Adapters
 * @details
 *
 */
class AdapterTester
{
    public:
        /// AdapterTester constructor.
        AdapterTester(const QString& name, const QString& xml);

        /// AdapterTester destructor.
        virtual ~AdapterTester();

        /// set a data file from which to read in test data
        //  the file is assumed to exist in the data directory
        //  of the test folder
        void setDataFile(const QString& filename);

        /// set any supporting service data
        void setServiceData(QHash<QString, DataBlob*>& serviceData);

        /// execute
        //  run deserialise() passing the set data stream
        //  with the number of specified bytes
        virtual void execute(DataBlob* blob);

    private:
        FactoryConfig<AbstractAdapter>* _adapterFactory;
        Config  _config;
        QIODevice* _device;
        QHash<QString, DataBlob*> _service;
        AbstractAdapter* _object;
        int _bytes;
};

} // namespace pelican
} // namespace pelican

#endif // ADAPTERTESTER_H
