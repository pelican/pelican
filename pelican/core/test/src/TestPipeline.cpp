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

#include "core/test/TestPipeline.h"
#include "core/test/EmptyModule.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

namespace pelican {
namespace test {

/**
 * @details
 * Default TestPipeline constructor.
 */
TestPipeline::TestPipeline(int iterations)
    : AbstractPipeline(),  _deactivateStop(false), _blobFactory(0)
{
    reset();
    _iterations = iterations;
}

/**
 * @details
 * Overloaded TestPipeline constructor.
 *
 * @param[in] requirements The data requirements of the pipeline.
 */
TestPipeline::TestPipeline(const DataRequirements& requirements, int iterations)
    : AbstractPipeline(), _deactivateStop(false), _blobFactory(0)
{
    reset();
    foreach ( const QString& type, requirements.allStreams() ) {
        requestRemoteData(type);
    }
    _iterations = iterations;
}

/**
 * @details
 * TestPipeline destructor.
 */
TestPipeline::~TestPipeline()
{
    delete _blobFactory;
}

/**
 * @details
 * Pipeline initialisation method (overloaded virtual).
 * Creates the modules in the pipeline.
 */
void TestPipeline::init()
{
}

/**
 * @details
 * Public reset method.
 */
void TestPipeline::reset()
{
    if( ! _blobFactory ) {
        _blobFactory = new FactoryGeneric<DataBlob>(true);
        setBlobFactory(_blobFactory);
    }
    _counter = 0;
    _matchedCounter = 0;
}

/**
 * @details
 * call deactivate rather than stop
 */
void TestPipeline::setDeactivation(bool d)
{
    _deactivateStop = d;
}

bool TestPipeline::deactivation() const {
    return _deactivateStop;
}

/**
 * @details
 * Pipeline run method (overridden virtual).
 * Defines a single iteration of the pipeline.
 */
void TestPipeline::run(QHash<QString, DataBlob*>& dataHash)
{
    // Print message.
    //cout << "Running TestPipeline, iteration " << _counter << endl;

    // Check the data is correct.
    if (_requiredDataRemote == dataHash.keys())
        ++_matchedCounter;

    // Increment counter and test for completion.
    if (++_counter >= _iterations)
    {
        if( _deactivateStop ) {
            deactivate();
        }
        else {
            stop(); // Stop the pipeline driver.
        }
    }
}

void TestPipeline::setHistory( const QString& stream, int size)
{
     _history[stream]=size;
}

} // namespace test
} // namespace pelican
