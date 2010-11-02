#ifndef DATABLOBWIDGET_H
#define DATABLOBWIDGET_H

/**
 * @file DataBlobWidget.h
 */

#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/FactoryConfig.h"

#include <QtGui/QWidget>

namespace pelican {

class DataBlob;

/**
 * @ingroup c_viewer
 *
 * @class DataBlobWidget
 *
 * @brief
 * Base class for DataBase Widget viewers
 *
 * @details
 */

class DataBlobWidget : public QWidget
{
    Q_OBJECT

    public:
        PELICAN_CONSTRUCT_TYPES(ConfigNode, QWidget*)

        DataBlobWidget( const ConfigNode& config, QWidget* parent=0 );
        virtual ~DataBlobWidget();
        virtual void updateData( DataBlob* data);
};

PELICAN_DECLARE(DataBlobWidget, DataBlobWidget)
} // namespace pelican
#endif // DATABLOBWIDGET_H
