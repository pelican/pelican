#ifndef OUTPUTSTREAMER_H
#define OUTPUTSTREAMER_H


#include "pelican/modules/AbstractModule.h"

/**
 * @file OutputStreamer.h
 */

namespace pelican {
    class AbstractBlobServer;
    class DataBlob;

/**
 * @class OutputStreamer
 *  
 * @brief
 *   Send Data Blobs to the current streaming mechanism
 * @details:
 *
 */

class OutputStreamer : public AbstractModule
{
    public:
        OutputStreamer(const ConfigNode config);
        ~OutputStreamer();
        void send(DataBlob*);

    private:
       AbstractBlobServer* _server;
       QString _name;  // the name to associate with this data
};

} // namespace pelican
#endif // OUTPUTSTREAMER_H 
