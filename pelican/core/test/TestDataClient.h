#ifndef TESTDATACLIENT_H
#define TESTDATACLIENT_H

/**
 * @file TestDataClient.h
 */

#include "pelican/core/AbstractDataClient.h"
#include "pelican/core/DataTypes.h"


namespace pelican {
namespace test {

/**
 * @ingroup t_core
 *
 * @class TestDataClient
 *
 * @brief
 * A test data client for use with the pipeline driver test.
 *
 * @details
 */

class TestDataClient : public AbstractDataClient
{
    public:
        /// Constructs a new test data client.
        TestDataClient(const ConfigNode& config, const DataTypes& types);

        /// Destroys the test data client.
        ~TestDataClient();

        /// Implements the getData() method.
        QHash<QString, DataBlob*> getData(QHash<QString, DataBlob*>&);
};

} // namepace test
} // namespace pelican

#endif // TESTDATACLIENT_H
