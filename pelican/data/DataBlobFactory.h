#ifndef DATABLOBFACTORY_H
#define DATABLOBFACTORY_H

#include <QString>
#include "pelican/utility/FactoryGeneric.h"
#include "pelican/data/DataBlob.h"

/**
 * @file DataBlobFactory.h
 */

namespace pelican {
    class Config;
    class DataBlob;

/**
 * @class DataBlobFactory
 * 
 * @brief
 *    Create DataBlobs on request
 * @details
 * 
 */
class DataBlobFactory : public FactoryGeneric<DataBlob>
{
    public:
        /// DataBlobFactory constructor.
        DataBlobFactory();

        /// DataBlobFactory destructor.
        ~DataBlobFactory();

    private:
};

} // namespace pelican

#endif // DATABLOBFACTORY_H 
