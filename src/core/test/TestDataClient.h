#ifndef TESTDATACLIENT_H
#define TESTDATACLIENT_H

#include "core/AbstractDataClient.h"
#include <QSet>

/**
 * @file TestDataClient.h
 */

namespace pelican {

/**
 * @class TestDataClient
 *  
 * @brief
 * 
 * @details
 * 
 */
class TestDataClient : public AbstractDataClient
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
