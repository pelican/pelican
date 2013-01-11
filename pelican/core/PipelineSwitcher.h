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

#ifndef PIPELINESWITCHER_H
#define PIPELINESWITCHER_H
#include <QtCore/QList>


/**
 * @file PipelineSwitcher.h
 */

namespace pelican {
class AbstractPipeline;

/**
 * @class PipelineSwitcher
 *
 * @brief
 *     Provides a way of switching between alternative pipelines
 * @details
 *     This is a container class of suitable pipelines that should
 *     be cycled through. The pipelines will be iterated through in the
 *     order added. Each new pipeline will be installed whenever the
 *     previous one is disabled
 *
 */
class PipelineSwitcher
{
    public:
        /// PipelineSwitcher constructor.
        PipelineSwitcher(bool cycle=true);

        /// PipelineSwitcher destructor.
        ~PipelineSwitcher();

        /// add the next pipeline to be run
        void addPipeline(AbstractPipeline*);

        /// return the list of all pipelines in the switcher
        const QList<AbstractPipeline*>& pipelines() const { return _pipelines; }

        /// return the next pipeline to be run
        AbstractPipeline* next();

    private:
        bool _cycle;
        int _current;
        QList<AbstractPipeline*> _pipelines;
};

} // namespace pelican

#endif // PIPELINESWITCHER_H
