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

#include <QtCore/QFile>

#include "AdapterTester.h"
#include "core/AbstractStreamAdapter.h"
#include "core/AbstractServiceAdapter.h"


namespace pelican {
namespace test {


/**
 * @details Constructs a AdapterTester object.
 */
AdapterTester::AdapterTester(const QString& type, const QString& config)
   : _device(0)
{
    // Setup the XML configuration.
    Config::TreeAddress adapterBase;
    adapterBase << Config::NodeId("adapters", "");
    QString xml =
            "<adapters><" + type + ">"
          + config + "\n</" + type + ">\n</adapters>";
    _config.setXML(xml);
    _adapterFactory = new FactoryConfig<AbstractAdapter>(&_config, adapterBase );
    _object = _adapterFactory->create(type,"");
}

/**
 * @details Destroys the AdapterTester object.
 */
AdapterTester::~AdapterTester()
{
     delete _device;
     delete _adapterFactory; // _object gets deleted with factory
}

void AdapterTester::setDataFile(const QString& filename)
{
     if( _device )
        delete _device;
     _device = new QFile(filename);
     if( ! _device->open( QIODevice::ReadOnly) ) {
        throw( QString("cannot open file" + filename + " for reading") );
     }
     _bytes = _device->size();
}

void AdapterTester::setServiceData(QHash<QString, DataBlob*>& serviceData)
{
    _service = serviceData;
}

void AdapterTester::execute(DataBlob* blob )
{
     switch( _object->type() ) {
         case AbstractAdapter::Stream:
             ((AbstractStreamAdapter*)(_object))->config(blob, _bytes, _service);
             break;
         case AbstractAdapter::Service:
             ((AbstractServiceAdapter*)_object)->config(blob, _bytes);
             break;
         default:
             throw( QString("unknown adapter type :%1").arg( _object->type()) ) ;
     }
     _object->deserialise(_device);
}

} // test
} // namespace pelican
