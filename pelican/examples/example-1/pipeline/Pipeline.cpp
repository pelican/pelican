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
#include "ServiceData.hpp"

#include <iostream>

using namespace std;

Pipeline::Pipeline()
: AbstractPipeline(), amplifier_(0), dataOut_(0), gains_(0), count_(0)
{
}

void Pipeline::init()
{
    requestRemoteData("StreamData");
    requestRemoteData("ServiceData");
}

void Pipeline::run(QHash<QString, pelican::DataBlob*>& data)
{
    cout << "Pipeline::run() start. [" << count_ << "]" << endl;

    StreamData* stream = (StreamData*) data["StreamData"];
    ServiceData* service = (ServiceData*) data["ServiceData"];

    cout << "--> stream size  = " << stream->size() << endl;
    cout << "    value[0]     = " << stream->ptr()[0] << endl;
    cout << "    value[1]     = " << stream->ptr()[1] << endl;
    cout << "    value[2]     = " << stream->ptr()[2] << endl;
    cout << "    value[3]     = " << stream->ptr()[3] << endl;
    cout << "    value[4]     = " << stream->ptr()[4] << endl;
    cout << "--> service size = " << service->size() << endl;
    cout << "    value[0]     = " << service->ptr()[0] << endl;

    ++count_;
    cout << "Pipeline::run() complete. " << endl << endl;
}
