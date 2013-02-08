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

#include "PelicanTimeRecorder.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <QtCore/QMutexLocker>

namespace pelican {

PelicanTimeRecorder* PelicanTimeRecorder::_globalTimer = 0;

/**
 * @details Constructs a PelicanTimeRecorder object.
 */
PelicanTimeRecorder::PelicanTimeRecorder( int reportInterval )
    : _reportInterval(reportInterval)
{
    _count = 0;
}

/**
 * @details Destroys the PelicanTimeRecorder object.
 */
PelicanTimeRecorder::~PelicanTimeRecorder()
{
}

void PelicanTimeRecorder::setReportInterval(int rl)
{
    _reportInterval = rl;
}

PelicanTimeRecorder* PelicanTimeRecorder::instance() {
    if( !  _globalTimer )
        _globalTimer = new PelicanTimeRecorder;
    return _globalTimer;
}

void PelicanTimeRecorder::start()
{
    QMutexLocker loc(&_mutex);
    _count = 0;
    _ticks.clear();
    _timer.start();
}

void PelicanTimeRecorder::tick(const QString& id)
{
    _ticks[id].push_back( _timer.elapsed() );
    if( _reportInterval && (++_count)%_reportInterval == 0)  report();
    _timer.restart();
}

void PelicanTimeRecorder::report() const
{
    foreach(const QString& tag, _ticks.keys() ) {
        if( _ticks[tag].size() > 0 ) {
            std::vector<unsigned int> temp = _ticks[tag];
            int max = *(std::max_element(temp.begin(), temp.end() ));
            int min = *(std::min_element(temp.begin(), temp.end() ));
            std::cout << tag.toStdString()
                << ": mean=" << std::accumulate( temp.begin(), temp.end(), 0 )
                / _ticks[tag].size();
            std::cout << ": max=" << max << ": min=" << min
                << " (ms) " << _ticks[tag].size()
                << " measurements" << std::endl;
        }
    }
}

} // namespace pelican
