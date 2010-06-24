#ifndef OUTPUTSTREAMMANAGER_H
#define OUTPUTSTREAMMANAGER_H

#include <QString>
#include <QMap>
#include <QList>

#include "pelican/output/AbstractOutputStream.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

/**
 * @file OutputStreamManager.h
 */

namespace pelican {

class DataBlob;

/**
 * @class OutputStreamManager
 *  
 * @brief
 *   Manages the output of data from the pipeline
 * @details
 *   Data output streams are managed by this object and, any data sent will be routed through to 
 *   the correct output channel
 */

class OutputStreamManager
{
    public:
        OutputStreamManager( const Config* config, const Config::TreeAddress& base );
        ~OutputStreamManager();

        /// send data to all relevant outputs on the specified stream
        void send( DataBlob* data, const QString& stream );

        /// associate an outputer to a specific data stream
        void addOutputStream(const QString& stream, AbstractOutputStream* streamer);

    private:
        FactoryConfig<AbstractOutputStream>* _factory;
        QMap< QString, QList<AbstractOutputStream*> > _streamers;

};

} // namespace pelican
#endif // OUTPUTSTREAMMANAGER_H 
