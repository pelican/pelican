#ifndef DATABLOBRELAY_H
#define DATABLOBRELAY_H

#include <QtCore/QObject>
#include "pelican/utility/Config.h"

/**
 * @file DataBlobRelay.h
 */

namespace pelican {
class OutputStreamManager;
class DataBlobClient;
class Stream;

/**
 * @class DataBlobRelay
 *  
 * @brief
 *    Connect to a DataBlob server at one end and redistribute
 *    via an OutputManager
 * @details
 *    
 */

class DataBlobRelay : public QObject
{
    Q_OBJECT

    public:
        DataBlobRelay( const Config* config, const Config::TreeAddress& address );
        ~DataBlobRelay();

        // add a client to the relay
        void addClient( DataBlobClient* client );

    private slots:
        void _streamData( const Stream& );

    private:
        OutputStreamManager* _outputManager;
        QList<DataBlobClient*> _clients;
        
};

} // namespace pelican
#endif // DATABLOBRELAY_H 
