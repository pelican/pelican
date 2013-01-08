#ifndef DATASUPPORTRESPONSE_H
#define DATASUPPORTRESPONSE_H

/**
 * @file DataSupportResponse.h
 */

#include "ServerResponse.h"
#include "data/DataSpec.h"

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
        DataSupportResponse(const DataSpec& spec );
        DataSupportResponse(const QSet<QString>& streams, const QSet<QString>& service = QSet<QString>() );
        ~DataSupportResponse();

        /// set the default adapter types for each stream
        void setDefaultAdapters( const QHash<QString, QString>& a ) { _spec.addAdapterTypes(a); }

        const QSet<QString>& streamData() const { return _spec.streamData(); }
        const QSet<QString>& serviceData() const { return _spec.serviceData(); }

        /// return a hash of Adapter types for each stream to use as a default
        //  if not overidden elsewhere
        const QHash<QString, QString>& defaultAdapters() const { return _spec.getAdapterTypes(); }

    private:
         DataSpec _spec;
};

} // namespace pelican
#endif // DATASUPPORTRESPONSE_H
