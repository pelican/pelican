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

#ifndef OUTPUTSTREAMMANAGER_H
#define OUTPUTSTREAMMANAGER_H

/**
 * @file OutputStreamManager.h
 */

#include "output/AbstractOutputStream.h"
#include "utility/FactoryConfig.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"

// include the basic types available for all pelican users
// to ensure the factory is aware of them
#include "output/PelicanTCPBlobServer.h"
#include "output/DataBlobFile.h"

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>

namespace pelican {

class DataBlob;

/**
 * @ingroup c_output
 *
 * @class OutputStreamManager
 *
 * @brief
 *   Manages the output of data from the pipeline
 * @details
 *   Data output streams are managed by this object and, any data sent will be routed through to
 *   the correct output channel. Any streamer added to this object will become owned by this object.
 *
 *   Configuration xml format:
 *   @code
 *   <output>
 *     <streamers>
 *        <PelicanTCPBlobServer active="true" >
 *            <connection port="1234" />
 *        <PelicanTCPBlobServer/>
 *        <FileStreamer name="networkdrive">
 *              <Directory path="/share/dataout" />
 *        </FileStreamer>
 *        <CustomStreamer active="true">
 *        </CustomStreamer>
 *     </streamers>
 *     <dataStreams>
 *        <Stream name="all" listeners="TCP_BlobServer" />
 *        <Stream name="MyData" listeners="networkdrive,CustomStreamer" />
 *     </dataStreams>
 *   </output>
 *   @endcode
 */

class OutputStreamManager
{
    public:
        OutputStreamManager( const Config* config, const Config::TreeAddress& base );
        ~OutputStreamManager();

        /// send data to all relevant outputs on the specified stream
        void send( const DataBlob* data, const QString& stream );

        /// associate an output streamer to a specific data stream
        void connectToStream( AbstractOutputStream* streamer, const QString& stream);

        /// show the number of
        QList<AbstractOutputStream*> connected(const QString& stream) const;

    private:
        FactoryConfig<AbstractOutputStream>* _factory;
        QMap< QString, QList<AbstractOutputStream*> > _streamers;

};

} // namespace pelican
#endif // OUTPUTSTREAMMANAGER_H
