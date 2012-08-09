#ifndef DATABLOBCHUNKERCLIENT_H
#define DATABLOBCHUNKERCLIENT_H

#include <QtCore/QByteArray>
#include "AbstractDataBlobClient.h"

/**
 * @file DataBlobChunkerClient.h
 */

namespace pelican {
class DataBlobChunker;
class AbstractClientProtocol;

/**
 * @class DataBlobChunkerClient
 *  
 * @brief
 *    A DataBlobClient for use with the server chunking mechanism
 * @details
 * 
 */

class DataBlobChunkerClient : public AbstractDataBlobClient
{
    Q_OBJECT

    public:
        DataBlobChunkerClient( AbstractClientProtocol*, DataBlobChunker* );
        virtual ~DataBlobChunkerClient();

        // overriden base class methods
        virtual void dataReceived( DataBlobResponse* res );

    private:
        DataBlobChunker* _chunker;
        QByteArray _byteArray;
};

} // namespace pelican
#endif // DATABLOBCHUNKERCLIENT_H 
