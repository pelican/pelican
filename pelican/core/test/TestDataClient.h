#ifndef TESTDATACLIENT_H
#define TESTDATACLIENT_H


#include "AbstractDataClient.h"

/**
 * @file TestDataClient.h
 */

namespace pelican {

/**
 * @class TestDataClient
 *  
 * @brief
 * A test data client for use with the pipeline driver test.
 * 
 * @details
 * 
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

    private:
};

} // namespace pelican

#endif // TESTDATACLIENT_H 
