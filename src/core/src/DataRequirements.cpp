#include "DataRequirements.h"
#include <QHash>
#include <QtGlobal>

namespace pelican {

/**
 * @details
 * DataRequirements constructor.
 */
DataRequirements::DataRequirements() 
    : _hash(0)
{
}

/**
 * @details
 * DataRequirements destructor.
 */
DataRequirements::~DataRequirements()
{
}

/**
 * @details
 * Sets the required stream data string list.
 */
void DataRequirements::setStreamData(const QString& string)
{
    setStreamData(QStringList(string));
}

/**
 * @details
 * Sets the required service data string list.
 */
void DataRequirements::setServiceData(const QString& string)
{
    setServiceData(QStringList(string));
}

/**
 * @details
 * Sets the required stream data string list.
 */
void DataRequirements::setStreamData(const QStringList& list)
{
    _hash = 0; // Invalidate the current hash.
    _streamData = list;
}

/**
 * @details
 * Sets the required service data string list.
 */
void DataRequirements::setServiceData(const QStringList& list)
{
    _hash = 0; // Invalidate the current hash.
    _serviceData = list;
}

/**
 * @details
 * Computes a hash of the requirements lists for use as a key with QHash.
 */
uint DataRequirements::hash() const
{
    if (_hash == 0) {
        /* Sort the requirements lists before recomputing the hash */
        QStringList streamCopy = _streamData;
        QStringList serviceCopy = _serviceData;
        streamCopy.sort();
        serviceCopy.sort();
        _hash = qHash(streamCopy.join(QString()) + serviceCopy.join(QString()));
    }
    return _hash;
}

/**
 * @details
 * Merges the given data requirements with those already known to this
 * class, removing any duplicates.
 */
DataRequirements& DataRequirements::operator+=(const DataRequirements& d)
{
    _streamData += d.streamData();
    _streamData.removeDuplicates();
    _serviceData += d.serviceData();
    _serviceData.removeDuplicates();
    return *this;
}

/**
 * @details
 * Tests if this requirements object is the same as the other one.
 */
bool DataRequirements::operator==(const DataRequirements& d) const
{
    return hash() == d.hash();
}

/**
 * @details
 * Provides a hash value for the DataRequirements object for use with QHash.
 */
uint qHash(const DataRequirements& key)
{
    return key.hash();
}

} // namespace pelican
