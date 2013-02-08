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

#ifndef PELICANTIMER_H
#define PELICANTIMER_H

#include <QtCore/QTime>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <vector>
#include <map>

/**
 * @file PelicanTimeRecorder.h
 */

namespace pelican {

/**
 * @class PelicanTimeRecorder
 *
 * @brief
 *    A time check reporting class
 * @details
 *    Use for profiling your code in integration tests etc.
 */
class PelicanTimeRecorder
{
    public:
        /// PelicanTimeRecorder constructor.
        PelicanTimeRecorder( int reportInterval = 0);

        /// PelicanTimeRecorder destructor.
        ~PelicanTimeRecorder();

        /// return the instance of the PelicanTimeRecorder
        static PelicanTimeRecorder* instance();

        /// set the number of calls between reports
        void setReportInterval(int rl);

        /// mark the time since the last tick call
        //  and associate this with the given reporting id
        void tick(const QString& id);

        /// start/reset the timer
        void start();

        /// output the report data
        //  foe each reporting id the tick times will
        //  be calculated and printed to stdout
        void report() const;

    private:
        typedef QMap<QString,std::vector<unsigned int> > T_Map;
        mutable QMutex _mutex;
        static PelicanTimeRecorder* _globalTimer;
        int _reportInterval;
        int _count;
        QTime _timer;
        T_Map _ticks;
};

extern PelicanTimeRecorder* __globalTimeRecorder;

} // namespace pelican

#endif // PELICANTIMER_H
