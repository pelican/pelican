#ifndef DATAREQUIREMENTS_H
#define DATAREQUIREMENTS_H

#include <QSet>
#include <QString>

/**
 * @file DataRequirements.h
 */

/**
 * @class DataRequirements
 *  
 * @brief
 * Specifies the types of data required by the pipeline modules
 * to operate a pipeline.
 * 
 * @details
 * Streaming data and service data specifications.
 */

namespace pelican {

class DataRequirements
{
    public:
        /// Constructs the data requirements object.
        DataRequirements();

        /// Destroys the data requirements object.
        ~DataRequirements();

        /// Sets the required stream data string list.
        void setStreamData(const QString& string);

        /// Sets the required service data string list.
        void setServiceData(const QString& string);

        /// Sets the required stream data string list.
        void setStreamData(const QSet<QString>& list);

        /// Sets the required service data string list.
        void setServiceData(const QSet<QString>& list);

        /// Returns the stream data.
        const QSet<QString> streamData() const {return _streamData;}

        /// Returns the service data.
        const QSet<QString> serviceData() const {return _serviceData;}

        /// Test for equality with another object.
        bool operator==(const DataRequirements&) const;

        /// Merge another requirements object with this one.
        DataRequirements& operator+=(const DataRequirements&);

        /// Compute a hash value for use with QHash.
        inline uint hash() const;

    private:
        mutable uint _hash;
        QSet<QString> _streamData;
        QSet<QString> _serviceData;
};

/// Compute a hash value for use with QHash (uses the hash member function).
uint qHash(const DataRequirements& key);

} // namespace pelican
#endif // DATAREQUIREMENTS_H 