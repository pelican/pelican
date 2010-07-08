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
        DataSupportResponse(  );
        ~DataSupportResponse();
        const QSet<QString>& streams() { return _streams; };

    private:
         QSet<QString> _streams;
};

} // namespace pelican
#endif // DATASUPPORTRESPONSE_H 
