#ifndef TESTDATACLIENT_H
#define TESTDATACLIENT_H

#include "DataClient.h"
#include <QSet>

/**
 * @file TestDataClient.h
 */

/**
 * @class TestDataClient
 *  
 * @brief
 * 
 * @details
 * 
 */

namespace pelican {

class TestDataClient : public DataClient
{
    public:
        TestDataClient(  );
        ~TestDataClient();

        QHash<QString, DataBlob*> getData(const DataRequirements& requirements);

        /// Sets the subset of the data to return.
        void setSubset(QSet<QString> set);

    private:
        QHash<QString, DataBlob*> _dataHash;
        QSet<QString> _subset;
};

} // namespace pelican
#endif // TESTDATACLIENT_H 
