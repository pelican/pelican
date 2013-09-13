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
#include <unistd.h>

using namespace std;

Pipeline::Pipeline() : AbstractPipeline(), runCounter_(0), reportCounter_(0),
        randDelay_(0)
{
}

void Pipeline::init()
{
    requestRemoteData("StreamData");
}

void Pipeline::run(QHash<QString, pelican::DataBlob*>& data)
{
    if (runCounter_ == 0)
        timer_.start();

    StreamData* stream = (StreamData*) data["StreamData"];

    quint32 packetSize = stream->packetSize();
    qint32 reportInterval = stream->reportInteval();

    ++runCounter_;

    // Report performance
    if (runCounter_%reportInterval == 0)
    {
        // Add a random delay to simulate some processing variation
        // TODO make this configurable though the emulation packet.
        int minDelay = 0;
        int maxDelay = randDelay_ < 20000 ? 40000 : 100;
        // random number in range from a to b (in microseconds)
        randDelay_ = rand() % maxDelay + minDelay;
        usleep(randDelay_);

        int elapsed = timer_.elapsed();
        size_t dataReceived = packetSize * reportInterval;
        const double B2MiB = 1.0/(1024.0*1024.0);
        char prefix[10];
        int l = (reportCounter_ < 10) ? 2 : ((reportCounter_ < 100) ? 1 : 0);
        sprintf(prefix, "P[%s%d] ", string(l, '-').c_str(), reportCounter_);

        printf("%s\n", string(80,'*').c_str());
        printf("%sTotal run count  = %i\n", prefix, runCounter_);
        printf("%sData blob size   = %-7.3f MiB [%i B]\n", prefix,
                packetSize*B2MiB, packetSize);
        printf("%sTotal data processed = %.1f MiB\n", prefix,
                (quint64)runCounter_*(quint64)packetSize*B2MiB);
        printf("%s\n", prefix);
        printf("%sReport Interval:\n", prefix);
        printf("%s* Pipeline runs  = %i\n", prefix, reportInterval);
        printf("%s* Data received  = %-7.3f MiB [%lu B]\n", prefix,
                dataReceived*B2MiB, dataReceived);
        printf("%s* Time taken     = %.3f s\n", prefix, elapsed*1.0e-3);
        printf("%s* Random delay   = %.6f s\n", prefix, randDelay_*1.0e-6);
        printf("%s* Data rate      = ", prefix);
        if (elapsed > 0)
            printf("%-7.3f MiB/s\n", dataReceived*B2MiB/(elapsed*1.0e-3));
        else
            printf("---     MiB/s\n");
        printf("%s\n\n", string(80,'*').c_str());
        fflush(stdout);
        ++reportCounter_;
        timer_.restart();
    }
}

