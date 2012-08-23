#ifndef DATABLOBADAPTER_H
#define DATABLOBADAPTER_H


#include "AbstractAdapter.h"

/**
 * @file DataBlobAdapter.h
 */

namespace pelican {

/**
 * @class DataBlobAdapter
 *  
 * @brief
 *    An Adapter to restore DataBlobs as passed by the
 * DataBlobChunker
 *
 * @details
 *    Reconstitutes a DataBlob from the incomming data stream 
 */

class DataBlobAdapter : public AbstractAdapter
{
    public:
        DataBlobAdapter( const ConfigNode& config );
        ~DataBlobAdapter();
        
        virtual void deserialise(QIODevice* in);
        
    private:
};

} // namespace pelican
#endif // DATABLOBADAPTER_H 
