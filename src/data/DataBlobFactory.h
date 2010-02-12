#ifndef DATABLOBFACTORY_H
#define DATABLOBFACTORY_H

#include <QString>

class QDomElement;

/**
 * @file DataBlobFactory.h
 */

namespace pelican {

class DataBlob;

/**
 * @class DataBlobFactory
 *  
 * @brief
 * Factory to create data blobs.
 * 
 * @details
 * This factory creates data blobs of the required type. It does <b>NOT</b>
 * own the data blobs, so they must be deleted by the abstract data client.
 */
class DataBlobFactory
{
    public:
        /// Data blob factory constructor.
        DataBlobFactory();

        /// Data blob factory destructor.
        ~DataBlobFactory();

        /// Creates a new data blob.
        DataBlob* create(const QString& type);
};

} // namespace pelican

#endif // DATABLOBFACTORY_H
