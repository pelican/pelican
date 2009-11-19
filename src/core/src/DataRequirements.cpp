#include "DataRequirements.h"
#include <QHash>
#include <QtGlobal>
#include <QStringList>
#include "utility/memCheck.h"

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
    _hash = 0; // mark to regenerate hash
    _streamData.insert(string);
}

/**
 * @details
 * Sets the required service data string list.
 */
void DataRequirements::setServiceData(const QString& string)
{
    _hash = 0; // mark to regenerate hash
    _serviceData.insert(string);
}

/**
 * @details
 * Sets the required stream data string list.
 */
void DataRequirements::setStreamData(const QSet<QString>& list)
{
    _hash = 0; // Invalidate the current hash.
    _streamData = list;
}

/**
 * @details
 * Sets the required service data string list.
 */
void DataRequirements::setServiceData(const QSet<QString>& list)
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
        QStringList streamCopy = _streamData.values();
        QStringList serviceCopy = _serviceData.values();
        qSort(streamCopy.begin(), streamCopy.end() );
        qSort(serviceCopy.begin(), serviceCopy.end() );
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
    _streamData.unite(d.streamData());
    _serviceData.unite(d.serviceData());
    _hash = 0; // mark for rehashing
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
 * Tests if this requirements object is the same as the other one.
 */
bool DataRequirements::isCompatible(const QHash<QString, DataBlob*>& d) const
{
    DataRequirements r;
    r.setStreamData(d.keys().toSet());
    return hash() == r.hash();
}


/**
 * @details
 */
bool operator==(const DataRequirements& r, const QHash<QString, DataBlob*>& hash)
{
    return r.isCompatible(hash);
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
