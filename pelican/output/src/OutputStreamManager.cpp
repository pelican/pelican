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

#include <iostream>
#include <QtCore/QtDebug>
#include <QtCore/QStringList>
#include "OutputStreamManager.h"
#include "utility/Config.h"
#include "utility/ConfigNode.h"

namespace pelican {

/**
 *@details OutputStreamManager
 */
OutputStreamManager::OutputStreamManager( const Config* config , const Config::TreeAddress& base )
    : _factory(0)
{
    if( config )
    {
        // facotry to allow us to generate streamers from the XML
        Config::TreeAddress streamerBase(base);
        streamerBase << Config::NodeId("streamers","");
        _factory = new FactoryConfig<AbstractOutputStream>(config, streamerBase);

        // parse the config file
        ConfigNode c = config->get(base);

        // create any the output streamers defined
        QMap<QString, AbstractOutputStream*> localStreamers;
        QList<QString> inactive;
        const QDomElement& element = c.getDomElement();
        for(QDomElement streamers = element.firstChildElement("streamers"); !streamers.isNull(); streamers = streamers.nextSiblingElement("streamers"))
        {
            for(QDomElement n = streamers.firstChildElement(); !n.isNull(); n = n.nextSiblingElement())
            {
                // active is true by default - we only need to create the object if it is active
                QString id = n.tagName();
                if( n.hasAttribute("name") )
                    id = n.attribute("name");
                if(localStreamers.contains(id))
                    throw(QString("OutputStreamManager configuration error: Multiple OutputStreamers with the same \"name\" defined \"%1\" on line : %2").arg(id).arg(n.lineNumber()) );
                if( !n.hasAttribute("active") ||  n.attribute("active").toLower() == QString("true") ) {
                    AbstractOutputStream* streamer = _factory -> create(n.tagName(), n.attribute("name"));
                    if( ! streamer )
                        throw(QString("OutputStreamManager configuration error: Unknown OuputStreamer type \"%1\"").arg(n.tagName()) );
                    localStreamers[id] = streamer;
                }
                else {
                    inactive.append(id);
                }
            }
        }

        int streamCount = 0;
        // connect streamers to stream channels
        for(QDomElement dstream = element.firstChildElement("dataStreams"); !dstream.isNull(); dstream = dstream.nextSiblingElement("dataStreams"))
        {
            for(QDomElement stream = dstream.firstChildElement("stream"); !stream.isNull(); stream = stream.nextSiblingElement("stream"))
            {
                ++streamCount;
                if( ! stream.hasAttribute("name") )
                    throw( QString("OutputStreamManager configuration error: <stream> tag requires a \"name\" attribute on line : %1").arg( stream.lineNumber() ) );
                foreach( QString id, stream.attribute( "listeners" ).split(",") )  {
                    if( ! localStreamers.contains(id) && ! inactive.contains(id) ) {
                        throw( QString("OutputStreamManager configuration error: <stream> tag refers to unknown output streamer \"%s\"").arg(id) );
                    }
                    if( localStreamers.contains(id) )
                        connectToStream(localStreamers[id], stream.attribute("name"));
                }
            }
        }
        // sanity check to ensure we have some streams defined for active streamers
        if( streamCount == 0 && localStreamers.size() > 0 )
            throw(QString("OutputStreamManager configuration error: No <streams> defined (inside <dataStreams> tags), but active OutputStreamers are defined"));
    }
}

/**
 *@details
 */
OutputStreamManager::~OutputStreamManager()
{
    delete _factory; // the factory is assumed to delete the objects it created
}

/**
 * @details
 * registers a specific output stream to listen to a data specifc stream
 * The special stream "all" is reserved. Any streamer connected to "all" will
 * be associated with every stream.
 */
void OutputStreamManager::connectToStream(AbstractOutputStream* streamer, const QString& stream)
{
    if( stream == "all" ) {
        // connect all existing streams to this streamer
        QList<QString> tmp =  _streamers.keys();
        tmp.removeAll("all");
        foreach( const QString& stream, tmp ) {
            _streamers[stream].append(streamer);
        }
    }
    else if( ! _streamers.contains(stream) ) {
        // in the case of a new stream, we must
        // make sure that all streamers registered to listen to "all" streams
        // will listen to this one too
        foreach( AbstractOutputStream* out, _streamers["all"] ) {
            _streamers[stream].append(out);
        }
    }
    _streamers[stream].append(streamer);
}

void OutputStreamManager::send( const DataBlob* data, const QString& stream )
{
    if( _streamers.contains(stream) ) {
        foreach( AbstractOutputStream* out, _streamers[stream]) {
            out->send(stream, data);
        }
    }
}

QList<AbstractOutputStream*> OutputStreamManager::connected(const QString& stream) const
{
    return _streamers[stream];
}

} // namespace pelican
