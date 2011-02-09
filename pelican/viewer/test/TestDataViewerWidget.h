#ifndef TESTDATAVIEWERWIDGET_H
#define TESTDATAVIEWERWIDGET_H

/**
 * @file TestDataViewerWidget.h
 */

#include "pelican/viewer/DataBlobWidget.h"

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
