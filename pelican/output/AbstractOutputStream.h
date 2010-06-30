#ifndef ABSTRACTOUTPUTSTREAM_H
#define ABSTRACTOUTPUTSTREAM_H

#include <QString>
#include "pelican/utility/FactoryRegistrar.h"

/**
 * @file AbstractOutputStream.h
 */

namespace pelican {
    class ConfigNode;
    class DataBlob;

/**
 * @class AbstractOutputStream
 *  
 * @brief
 *    Base class for all data output modules
 * @details
 *    Need to implement the send() method to export data
 *    as required
 */

class AbstractOutputStream
{
    public:
        PELICAN_CONSTRUCT_TYPES(ConfigNode)

        AbstractOutputStream( const ConfigNode& configNode );
        virtual ~AbstractOutputStream();

        /// Will be called with data to be streamed
        virtual void send(const QString& streamName, const DataBlob* dataBlob) = 0;

    private:
};

} // namespace pelican
#endif // ABSTRACTOUTPUTSTREAM_H 
