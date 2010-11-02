#ifndef TESTDATABLOBCLIENT_H
#define TESTDATABLOBCLIENT_H

/**
 * @file TestDataBlobClient.h
 */

#include "pelican/output/AbstractDataBlobClient.h"
#include <QtCore/QSet>

namespace pelican {
namespace test {

/**
 * @ingroup t_output
 *
 * @class TestDataBlobClient
 *
 * @brief
 * A dummy DatBlobClient for unit testing
 *
 * @details
 */

class TestDataBlobClient : public AbstractDataBlobClient
{
    Q_OBJECT

    public:
        /// TestDataBlobClient constructor.
        TestDataBlobClient();

        /// TestDataBlobClient destructor.
        ~TestDataBlobClient();

        /// emits signals and send Data
        void send(const Stream&);

        /// return the set of all streams that have been subscribed to
        const QSet<QString>& subscriptions() const;

        // interface requirements
        virtual QSet<QString> streams();
        virtual void subscribe( const QSet<QString>& streams );

    private:
        QSet<QString> _streams;
        QSet<QString> _subs;
};

} // namespace test
} // namespace pelican
#endif // TESTDATABLOBCLIENT_H
