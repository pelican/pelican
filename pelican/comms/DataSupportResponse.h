#ifndef DATASUPPORTRESPONSE_H
#define DATASUPPORTRESPONSE_H

/**
 * @file DataSupportResponse.h
 */

#include "ServerResponse.h"

#include <QtCore/QSet>
#include <QtCore/QString>

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class DataSupportResponse
 *
 * @brief
 * Contains a list of supported streams.
 *
 * @details
 */

class DataSupportResponse : public ServerResponse
{
    public:
        DataSupportResponse(const QSet<QString>& streams,
                const QSet<QString>& serviceStreams = QSet<QString>());
        ~DataSupportResponse();
        const QSet<QString>& streamData() const { return _streams; };
        const QSet<QString>& serviceData() const { return _services; };

    private:
         QSet<QString> _streams;
         QSet<QString> _services;
};

} // namespace pelican
#endif // DATASUPPORTRESPONSE_H
