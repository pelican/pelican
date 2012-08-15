#ifndef DATABLOBCHUNKER_H
#define DATABLOBCHUNKER_H


#include "AbstractChunker.h"

/**
 * @file DataBlobChunker.h
 */

namespace pelican {
class DataBlobChunkerClient;

/**
 * @class DataBlobChunker
 *  
 * @brief
 *    A pass-through chunker for DataBlobs emitted from a 
 *    Pelican Output Streaming server
 * @details
 *    Each chunk corresponds to a single DataBlob
 */

class DataBlobChunker : public AbstractChunker
{
        friend class DataBlobChunkerClient;

    public:
        DataBlobChunker(const ConfigNode& config);
        ~DataBlobChunker();

        virtual QIODevice* newDevice();
        virtual void next(QIODevice*) {};

        void setPort(quint16 port);
        quint16 port() const;
        void setHost(const QString& ipaddress);

    private:
        DataBlobChunkerClient* _client;

};
PELICAN_DECLARE_CHUNKER(DataBlobChunker)

} // namespace pelican
#endif // DATABLOBCHUNKER_H 
