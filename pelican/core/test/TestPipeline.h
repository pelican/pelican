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

#ifndef TESTPIPELINE_H
#define TESTPIPELINE_H

/**
 * @file TestPipeline.h
 */

#include "core/AbstractPipeline.h"
#include "data/DataRequirements.h"

namespace pelican {
namespace test {


/**
 * @ingroup t_core
 *
 * @class TestPipeline
 *
 * @brief
 * This class tests methods of AbstractPipeline by inheriting it.
 *
 * @details
 * The TestPipeline class is used as the basis of unit testing the
 * AbstractPipeline class, of which this is a friend.
 */

class TestPipeline : public AbstractPipeline
{
    private:
        int _iterations;
        int _counter;
        int _matchedCounter;
        bool _deactivateStop;
        FactoryGeneric<DataBlob>* _blobFactory;

    public:
        /// Default constructor.
        TestPipeline(int iterations = 0);

        /// Overloaded constructor.
        TestPipeline(const DataRequirements& requirements, int iterations = 0);

        /// Destructor.
        ~TestPipeline();

        /// Reads counter.
        int count() const {return _counter;}

        /// Initialises the pipeline.
        void init();

        /// Reads counter used to check that the data passed is the same as the
        /// expected data.
        int matchedCounter() const {return _matchedCounter;}

        /// return the deactivation setting
        bool deactivation() const;

        /// set the hisrory size for the specified stream
        void setHistory( const QString& stream, int size);

        /// Reset counters.
        void reset();
        void setDeactivation(bool v=true);

        /// Runs the pipeline.
        void run(QHash<QString, DataBlob*>& dataHash);
};

} // namespace test
} // namespace pelican

#endif // TESTPIPELINE_H
