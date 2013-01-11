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

#ifndef PELICANTIMER_H_
#define PELICANTIMER_H_

#include <ctime>
#include <cstdio>
#include "sys/time.h"

#include <QtCore/QTime>

/**
 * @ingroup c_utility
 *
 * @brief
 * Preprocessor macros used for timing.
 *
 * @details
 * This header defines two macros to measure the time interval
 * taken between their calls.
 *
 * Define TIMER_ENABLE to use them.
 *
 * Call TIMER_START to start the timer, and TIMER_STOP(...) to print the time
 * taken (with a message prefix) to standard output (STDOUT). The timer uses
 * QTime and therefore has millisecond accuracy.
 *
 * To use a CPU clock timer instead, call CLOCK_START and CLOCK_STOP(...)
 * instead.
 */
#ifdef TIMER_ENABLE
#define TIMER_START {QTime _t; _t.start();
#define TIMER_TICK(...) fprintf(stdout, "\n"); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, ": %.3f sec.\n", _t.elapsed() / 1000.0);
#define TIMER_STOP(...) fprintf(stdout, "\n"); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, ": %.3f sec.\n", _t.elapsed() / 1000.0);}
#else
#define TIMER_START ;
#define TIMER_STOP(...) ;
#define TIMER_TICK(...) ;
#endif

#ifdef TIMER_ENABLE
#define CLOCK_START {clock_t _pelican_start_time = clock();
#define CLOCK_STOP(...) clock_t _pelican_timer_interval = clock() - _pelican_start_time; \
    float _pelican_time_sec = (float)(_pelican_timer_interval) / CLOCKS_PER_SEC; \
    fprintf(stdout, "\n"); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, ": %.2f sec.\n", _pelican_time_sec);}
#else
#define CLOCK_START ;
#define CLOCK_STOP(...) ;
#endif


#ifdef TIMER_ENABLE
    #define TIMER2_START {timeval _t1; gettimeofday(&_t1, NULL);
    #define TIMER2_STOP(...) timeval _t2; gettimeofday(&_t2, NULL); \
        double _start = _t1.tv_sec + _t1.tv_usec * 1.0e-6; \
        double _end = _t2.tv_sec + _t2.tv_usec * 1.0e-6; \
        fprintf(stdout, "\n"); \
        fprintf(stdout, __VA_ARGS__); \
        fprintf(stdout, ": %.3f sec.\n", _end - _start);};
#else
    #define TIMER2_START ;
    #define TIMER2_STOP(...) ;
#endif

#endif // PELICANTIMER_H_
