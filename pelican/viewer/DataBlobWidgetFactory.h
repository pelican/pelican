#ifndef DATABLOBWIDGETFACTORY_H
#define DATABLOBWIDGETFACTORY_H

/**
 * @file DataBlobWidgetFactory.h
 */

#include "pelican/utility/FactoryConfig.h"
#include "pelican/viewer/DataBlobWidget.h"

namespace pelican {
class Config;
class DataBlobWidget;

/**
 * @ingroup c_viewer
 *
 * @class DataBlobWidgetFactory
 *
 * @brief
 * Create DataBlobWidgets on request
 *
 * @details
 */

class DataBlobWidgetFactory : public FactoryConfig<DataBlobWidget>
{
    public:
        /// DataBlobWidgetFactory constructor.
        DataBlobWidgetFactory(const Config* config,
                const Config::TreeAddress& base);

        /// DataBlobWidgetFactory destructor.
        ~DataBlobWidgetFactory();

        /// overloaded convenience operator for create
        DataBlobWidget* create(const QString&);
};

} // namespace pelican

#endif // DATABLOBWIDGETFACTORY_H
