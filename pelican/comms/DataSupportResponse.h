#ifndef DATASUPPORTRESPONSE_H
#define DATASUPPORTRESPONSE_H


#include <QSet>
#include <QString>
#include "ServerResponse.h"

/**
 * @file DataSupportResponse.h
 */

namespace pelican {

/**
 * @class DataSupportResponse
 *  
 * @brief
 *   contains a list of supported streams
 * @details
 * 
 */

class DataSupportResponse : public ServerResponse
{
    public:
        DataSupportResponse( const QSet<QString>& streams, const QSet<QString>& serviceStreams = QSet<QString>()  );
        ~DataSupportResponse();
        const QSet<QString>& streamData() const { return _streams; };
        const QSet<QString>& serviceData() const { return _services; };

    private:
         QSet<QString> _streams;
         QSet<QString> _services;
};

} // namespace pelican
#endif // DATASUPPORTRESPONSE_H 
