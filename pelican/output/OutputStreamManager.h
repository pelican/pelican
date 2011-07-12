#ifndef OUTPUTSTREAMMANAGER_H
#define OUTPUTSTREAMMANAGER_H

/**
 * @file OutputStreamManager.h
 */

#include "pelican/output/AbstractOutputStream.h"
#include "pelican/utility/FactoryConfig.h"
#include "pelican/utility/Config.h"
#include "pelican/utility/ConfigNode.h"

// include the basic types available for all pelican users
// to ensure the factory is aware of them
#include "pelican/output/PelicanTCPBlobServer.h"
#include "pelican/output/DataBlobFile.h"

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QList>

namespace pelican {

class DataBlob;

/**
 * @ingroup c_output
 *
 * @class OutputStreamManager
 *
 * @brief
 *   Manages the output of data from the pipeline
 * @details
 *   Data output streams are managed by this object and, any data sent will be routed through to
 *   the correct output channel. Any streamer added to this object will become owned by this object.
 *
 *   Configuration xml format:
 *   @code
 *   <output>
 *     <streamers>
 *        <PelicanTCPBlobServer active="true" >
 *            <connection port="1234" />
 *        <PelicanTCPBlobServer/>
 *        <FileStreamer name="networkdrive">
 *              <Directory path="/share/dataout" />
 *        </FileStreamer>
 *        <CustomStreamer active="true">
 *        </CustomStreamer>
 *     </streamers>
 *     <dataStreams>
 *        <Stream name="all" listeners="TCP_BlobServer" />
 *        <Stream name="MyData" listeners="networkdrive,CustomStreamer" />
 *     </dataStreams>
 *   </output>
 *   @endcode
 */

class OutputStreamManager
{
    public:
        OutputStreamManager( const Config* config, const Config::TreeAddress& base );
        ~OutputStreamManager();

        /// send data to all relevant outputs on the specified stream
        void send( const DataBlob* data, const QString& stream );

        /// associate an output streamer to a specific data stream
        void connectToStream( AbstractOutputStream* streamer, const QString& stream);

        /// show the number of
        QList<AbstractOutputStream*> connected(const QString& stream) const;

    private:
        FactoryConfig<AbstractOutputStream>* _factory;
        QMap< QString, QList<AbstractOutputStream*> > _streamers;

};

} // namespace pelican
#endif // OUTPUTSTREAMMANAGER_H
