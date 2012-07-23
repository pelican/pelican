#ifndef DATABLOBRELAY_H
#define DATABLOBRELAY_H

#include <QtCore/QObject>
#include "pelican/utility/Config.h"

/**
 * @file DataBlobRelay.h
 */

namespace pelican {
class OutputStreamManager;
class AbstractOutputStream;
class AbstractDataBlobClient;
class DataBlobClient;
class Stream;

/**
 * @class DataBlobRelay
 *  
 * @brief
 *    Connect to a DataBlob server at one end and redistribute
 *    via an OutputManager
 * @details
 *    Input configuration takes the format:
 *    <output>
 *         See the OutputStreamManager for details
 *    </output>
 *    <client>
 *         See the DataBlobClient configuration for details
 *    </client>
 *    <client>
 *         ...
 *    </client>
 */

class DataBlobRelay : public QObject
{
    Q_OBJECT

    public:
        DataBlobRelay( const Config* config, const Config::TreeAddress& address );
        ~DataBlobRelay();

        // add a client to the relay
        void addClient( AbstractDataBlobClient* client );

        /// associate an output streamer to a specific data stream
        void connectToStream( AbstractOutputStream* streamer, const QString& stream);

    private slots:
        void _streamData( const Stream& );

    private:
        OutputStreamManager* _outputManager;
        QList<AbstractDataBlobClient*> _clients;   // clients to listen to
        QList<DataBlobClient*> _myClients; // clients to delete
        
};

} // namespace pelican
#endif // DATABLOBRELAY_H 
