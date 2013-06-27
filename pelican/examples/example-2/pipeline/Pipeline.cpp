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

#include "Pipeline.hpp"
#include "StreamData.hpp"
#include <iostream>

using namespace std;

Pipeline::Pipeline()
: AbstractPipeline(), counter_(0)
{
}

void Pipeline::init()
{
    requestRemoteData("StreamData");
}

void Pipeline::run(QHash<QString, pelican::DataBlob*>& data)
{
    if (counter_ == 0)
        timer_.start();

    StreamData* stream = (StreamData*) data["StreamData"];

//    quint32 timeStamp  = stream->timeStamp();
//    quint32 packetId   = stream->packetId();
//    quint32 numPackets = stream->totalPackets();
    quint32 packetSize = stream->packetSize();

    quint32 reportInterval = 50;
    if (counter_%reportInterval == 0 && counter_ > 0)
    {
        float elapsed = timer_.elapsed() / 1.0e3;
        float MiB = (packetSize * reportInterval) / (1024.0 * 1024.0);
        cout << endl;
        cout << string(80, '*') << endl;
        cout << __PRETTY_FUNCTION__ << endl;
        cout << std::string(80, '-') << endl;
        cout << "-- counter     = " << counter_ << endl;
        cout << "-- no. packets = " << reportInterval << endl;
        cout << "-- packet size = " << packetSize << " bytes" << endl;
        cout << "-- packet size = " << packetSize/(1024.0*1024.0) << " MiB" << endl;
        cout << "-- time taken  = " << elapsed << " seconds." << endl;
        cout << "-- MiB (sent)  = " << MiB << std::endl;
        cout << "-- MiB/s       = " << MiB/elapsed  << endl;
        cout << std::string(80, '*') << endl;
        cout << endl;
        timer_.restart();
    }

    ++counter_;
}



