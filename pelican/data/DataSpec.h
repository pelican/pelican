#ifndef DATA_SPEC_H
#define DATA_SPEC_H

/**
 * @file DataSpec.h
 */

#include <QtCore/QSet>
#include <QtCore/QString>

namespace pelican {

class DataBlob;

/**
 * @ingroup c_data
 *
 * @class DataSpec
 *
 * @brief
 * Specifies the types of data required by the pipeline modules
 * to operate a pipeline.
 *
 * @details
 * Streaming data and service data specifications.
 */

class DataSpec
{
    public:
        /// Constructs the data requirements object.
        DataSpec();

        /// Destroys the data requirements object.
        ~DataSpec();

    public:
        /// Returns a set containing all required data types.
        QSet<QString> allData() const;

        /// Return true if thew specified stream is available
        bool contains(const QString& stream) const;

        /// Adds the required service data type.
        void addServiceData(const QString& type);

        /// Adds the required service data set.
        void addServiceData(const QSet<QString>& list);

        /// Adds the required stream data type.
        void addStreamData(const QString& type);

        /// Adds the required stream data set.
        void addStreamData(const QSet<QString>& list);

        /// remove the specified stream data
        void removeStreamData(const QString& type);

        /// remove all data
        void clear();

        /// Compute a hash value for use with QHash.
        uint hash() const;

        /// Test for compatibility with another DataSpec.
        bool isCompatible(const DataSpec& d) const;

        /// Test for compatibility with a data blob hash.
        bool isCompatible(const QHash<QString, DataBlob*>& d) const;

        /// Ensure that the specified data is marked as service data
        void setServiceData(const QString& string);

        /// Ensure that the specified data is marked as stream data
        void setStreamData(const QString& string);

        /// Sets the required service data set.
        //void setServiceData(const QSet<QString>& list);

        /// Sets the required stream data set.
        //void setStreamData(const QSet<QString>& list);

        /// Returns the set of service data required.
        const QSet<QString>& serviceData() const {return _serviceData;}

        /// Returns the set of stream data required.
        const QSet<QString>& streamData() const {return _streamData;}

        /// Test for equality with another object.
        bool operator==(const DataSpec&) const;

        /// Test for inequality with another object.
        bool operator!=(const DataSpec&) const;

        /// Merge another requirements object with this one.
        DataSpec& operator+=(const DataSpec&);

        /// Merge two requirements objects and return the new result.
        const DataSpec operator+(const DataSpec&) const;

        /// return the number of data types required
        int size() const;

    private:
        mutable uint _hash;
        QSet<QString> _streamData;
        QSet<QString> _serviceData;
};

/// Test for compatibility with a data blob hash.
bool operator==(const DataSpec&, const QHash<QString, DataBlob*>&);

/// Compute a hash value for use with QHash (uses the hash member function).
uint qHash(const DataSpec& key);

} // namespace pelican
#endif // DATA_SPEC_H
