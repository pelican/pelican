#ifndef DATABLOBFACTORY_H
#define DATABLOBFACTORY_H

/**
 * @file DataBlobFactory.h
 */

#include "pelican/utility/FactoryGeneric.h"
#include "pelican/data/DataBlob.h"

#include <QtCore/QString>

namespace pelican {
class Config;
class DataBlob;

/**
 * @ingroup c_data
 *
 * @class DataBlobFactory
 *
 * @brief
 * Create DataBlobs on request
 *
 * @details
 */

class DataBlobFactory : public FactoryGeneric<DataBlob>
{
    public:
        /// DataBlobFactory constructor.
        DataBlobFactory();

        /// DataBlobFactory destructor.
        ~DataBlobFactory();
};

} // namespace pelican

#endif // DATABLOBFACTORY_H
