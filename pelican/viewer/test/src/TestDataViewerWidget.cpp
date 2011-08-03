#include "TestDataViewerWidget.h"

namespace pelican {
namespace test {

int TestDataViewerWidget::_count = 0;

/**
 * @details Constructs a TestDataViewerWidget object.
 */
TestDataViewerWidget::TestDataViewerWidget(  const ConfigNode& config, QWidget* parent )
    : DataBlobWidget(config , parent)
{
    _count = 0;
}

/**
 * @details Destroys the TestDataViewerWidget object.
 */
TestDataViewerWidget::~TestDataViewerWidget()
{
}

void TestDataViewerWidget::updateData( DataBlob* )
{
    ++_count;
}

} // namespace test
} // namespace pelican
