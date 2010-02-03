#ifndef PELICANSERVERCLIENT_H
#define PELICANSERVERCLIENT_H

#include "AbstractDataClient.h"

/**
 * @file PelicanServerClient.h
 */

namespace pelican {
    class AbstractClientProtocol;

/**
 * @class PelicanServerClient
 *  
 * @brief
 *    Implements the dataClient interface for attaching to a Pelican Server
 * @details
 * 
 */

class PelicanServerClient : public AbstractDataClient
{
    public:
        PelicanServerClient(  );
        ~PelicanServerClient();
        virtual QHash<QString, DataBlob*> getData(const DataRequirements&);

    protected:
        void getServiceData(QHash<QString,QString> requirements, QHash<QString, DataBlob*>& datahash);
        void _connect();

    private:
        AbstractClientProtocol* _protocol;
};

} // namespace pelican
#endif // PELICANSERVERCLIENT_H 
