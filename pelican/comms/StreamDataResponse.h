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

#ifndef STREAMDATARESPONSE_H
#define STREAMDATARESPONSE_H

/**
 * @file StreamDataResponse.h
 */

#include "comms/ServerResponse.h"
#include "comms/StreamData.h"

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class StreamDataResponse
 *
 * @brief
 * Class to represent a set of stream data returned from the server.
 *
 * @details
 */

class StreamDataResponse : public ServerResponse
{
    private:
        pelican::StreamData* _data;

    public:
        /// Constructs a StreamDataResponse object.
        StreamDataResponse();

        /// Destroys the StreamDataResponse object.
        ~StreamDataResponse();

        /// Sets the pointer to the StreamData object.
        void setStreamData(pelican::StreamData* data) {_data = data;}

        /// Returns the pointer to the StreamData object.
        pelican::StreamData* streamData() {return _data;}
};

} // namespace pelican
#endif // STREAMDATARESPONSE_H
