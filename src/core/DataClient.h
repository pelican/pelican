#ifndef DATACLIENT_H
#define DATACLIENT_H

#include <QHash>
#include <QString>

/**
 * @file DataClient.h
 */

/**
 * @class DataClient
 *  
 * @brief
 * This class is the data client for the pipeline driver.
 * 
 * @details
 * A pointer to a DataClient class is passed to the pipeline driver, which
 * uses it to fetch data from the data server.
 */

namespace pelican {

class DataBlob;
class DataRequirements;

class DataClient
{
    public:
        /// Data client constructor.
        DataClient(  );

        /// Data client destructor.
        ~DataClient();

        /// Gets the requested data from the data server.
        virtual QHash<QString, DataBlob*> getData(const DataRequirements&);

    private:
};

} // namespace pelican
#endif // DATACLIENT_H 
