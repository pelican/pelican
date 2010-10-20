#ifndef TESTDATAVIEWERWIDGET_H
#define TESTDATAVIEWERWIDGET_H


#include "pelican/viewer/DataBlobWidget.h"

/**
 * @file TestDataViewerWidget.h
 */

namespace pelican {

/**
 * @class TestDataViewerWidget
 * 
 * @brief
 *   iA dummuy widget used for Unit testing
 * @details
 * 
 */
class TestDataViewerWidget : public DataBlobWidget
{
    public:
        /// TestDataViewerWidget constructor.
        TestDataViewerWidget( const ConfigNode&, QWidget* parent = 0 );

        /// TestDataViewerWidget destructor.
        ~TestDataViewerWidget();

        /// returns the number of time update has been called
        //  (on all TestDataViewerWidget)
        //  since the last TestDataViewer object was created
        //  this counter is reset every new().
        static int updateCalled() { return _count; };

        virtual void updateData( DataBlob* data);

    private:
        static int _count;
};

PELICAN_DECLARE(DataBlobWidget, TestDataViewerWidget )

} // namespace pelican

#endif // TESTDATAVIEWERWIDGET_H 
