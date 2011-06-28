#ifndef ABSTRACTOUTPUTSTREAM_H
#define ABSTRACTOUTPUTSTREAM_H

#include "pelican/utility/FactoryRegistrar.h"
#include <QtCore/QString>

/**
 * @file AbstractOutputStream.h
 */

namespace pelican {
    class ConfigNode;
    class DataBlob;

/**
 * @ingroup c_output
 *
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
        void setVerbose(int level) { _verbose = level; };

        /// send the data
        void send(const QString& streamName, const DataBlob* dataBlob);

    protected:
        /// Will be called with data to be streamed
        virtual void sendStream(const QString& streamName, const DataBlob* dataBlob) = 0;

    protected:
        /// report verbose messages
        void verbose(const QString&, int level = 1);

    private:

    protected:
        int _verbose;
};

} // namespace pelican
#endif // ABSTRACTOUTPUTSTREAM_H
