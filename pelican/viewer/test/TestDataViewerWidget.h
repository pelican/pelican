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

#ifndef TESTDATAVIEWERWIDGET_H
#define TESTDATAVIEWERWIDGET_H

/**
 * @file TestDataViewerWidget.h
 */

#include "viewer/DataBlobWidget.h"

namespace pelican {
namespace test {

/**
 * @ingroup t_viewer
 *
 * @class TestDataViewerWidget
 *
 * @brief
 * A dummy widget used for Unit testing
 *
 * @details
 */

class TestDataViewerWidget : public DataBlobWidget
{
    public:
        /// TestDataViewerWidget constructor.
        TestDataViewerWidget( const ConfigNode&, QWidget* parent = 0 );

        /// TestDataViewerWidget destructor.
        ~TestDataViewerWidget();

        /// Returns the number of times update has been called.
        //  (on all TestDataViewerWidget)
        //  since the last TestDataViewer object was created
        //  this counter is reset every new().
        static int updateCalled() { return _count; };

        virtual void updateData( DataBlob* data);

    private:
        static int _count;
};

PELICAN_DECLARE(DataBlobWidget, TestDataViewerWidget )

} // namespace test
} // namespace pelican
#endif // TESTDATAVIEWERWIDGET_H
