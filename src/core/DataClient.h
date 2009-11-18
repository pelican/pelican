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
 * 
 * @details
 * 
 */

namespace pelican {

class DataBlob;
class DataRequirements;

class DataClient
{
    public:
        DataClient(  );
        ~DataClient();

        /// Gets the requested data from the server.
        QHash<QString, DataBlob*> getData(const DataRequirements&);

    private:
};

} // namespace pelican
#endif // DATACLIENT_H 
