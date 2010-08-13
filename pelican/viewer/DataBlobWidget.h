#ifndef DATABLOBWIDGET_H
#define DATABLOBWIDGET_H

#include <QtGui/QWidget>
#include "pelican/utility/ConfigNode.h"
#include "pelican/utility/FactoryConfig.h"

/**
 * @file DataBlobWidget.h
 */

namespace pelican {

class DataBlob;

/**
 * @class DataBlobWidget
 *
 * @brief
 *   Base class for DataBase Widget viewers
 * @details
 *
 */

class DataBlobWidget : public QWidget
{
    Q_OBJECT

    public:
        PELICAN_CONSTRUCT_TYPES(ConfigNode, QWidget*)

        DataBlobWidget( const ConfigNode& config, QWidget* parent=0 );
        virtual ~DataBlobWidget();
        virtual void updateData( DataBlob* data);

    private:
};

PELICAN_DECLARE(DataBlobWidget, DataBlobWidget)
} // namespace pelican
#endif // DATABLOBWIDGET_H
