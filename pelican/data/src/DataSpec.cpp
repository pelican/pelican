#include "pelican/data/DataSpec.h"
#include <QtCore/QHash>
#include <QtCore/QtGlobal>
#include <QtCore/QStringList>

namespace pelican {

/**
 * @details
 * DataSpec constructor.
 */
DataSpec::DataSpec()
    : _hash(0)
{
}

/**
 * @details
 * DataSpec destructor.
 */
DataSpec::~DataSpec()
{
}

/**
 * @details
 * Adds the given parameter to the required service data.
 */
void DataSpec::addServiceData(const QString& string)
{
    _hash = 0; // Mark for rehashing.
    _serviceData.insert(string);
}

/**
 * @details
 * Adds the given set to the service data requirements.
 */
void DataSpec::addServiceData(const QSet<QString>& list)
{
    _hash = 0; // Mark for rehashing.
    _serviceData.unite(list);
}

/**
 * @details
 * Adds the given parameter to the required stream data.
 */
void DataSpec::addStreamData(const QString& string)
{
    _hash = 0; // Mark for rehashing.
    _streamData.insert(string);
}

void DataSpec::removeStreamData(const QString& type)
{
    _hash = 0; // Mark for rehashing.
    _streamData.remove(type);
}

/**
 * @details
 * Adds the given set to the service data requirements.
 */
void DataSpec::addStreamData(const QSet<QString>& list)
{
    _hash = 0; // Mark for rehashing.
    _streamData.unite(list);
}

QSet<QString> DataSpec::allData() const {
#ifdef BROKEN_QT_SET_HEADER
    // The Qt header misses a const declaration so we have to make
    // a double copy
    QSet<QString> temp = _serviceData;
    return temp + _streamData;
#else
    return _serviceData + _streamData;
#endif
}

bool DataSpec::contains(const QString& stream) const {
     return _streamData.contains(stream) || _serviceData.contains(stream);
}

/**
 * @details
 * Clears all the current requirements.
 */
void DataSpec::clear()
{
    _hash = 0; // Mark for rehashing.
    _streamData.clear();
    _serviceData.clear();
    _adapterTypes.clear();
}

/**
 * @details
 * Computes a hash of the requirements lists for use as a key with QHash.
 */
uint DataSpec::hash() const
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
 * Tests if this requirements object is the same as or is a subset of another.
 */
bool DataSpec::isCompatible(const DataSpec& d) const
{
    QSet<QString> temp(_streamData);
    temp.unite(_serviceData);
    QSet<QString> tempd(d._streamData);
    tempd.unite(d._serviceData);
    temp.subtract(tempd);
    return (temp.size() == 0);
}

/**
 * @details
 * Tests if data specified in this requirements object is
 * contained within the passed hash.
 */
bool DataSpec::isCompatible(const QHash<QString, DataBlob*>& d) const
{
    QSet<QString> temp(_streamData);
    temp.unite(_serviceData);
    temp.subtract(d.keys().toSet());
    return (temp.size() == 0);
}

/**
 * @details
 */
void DataSpec::setServiceData(const QString& string)
{
    _hash = 0; // Mark for rehashing.
    if( _streamData.contains(string) )
        _streamData.remove(string);
    if( ! _serviceData.contains(string) )
        _serviceData.insert(string);
}

/**
 * @details
 */
void DataSpec::setStreamData(const QString& string)
{
    _hash = 0; // Mark for rehashing.
    if( _serviceData.contains(string) )
        _serviceData.remove(string);
    if( ! _streamData.contains(string) )
        _streamData.insert(string);
}

/**
 * @details
void DataSpec::setServiceData(const QSet<QString>& list)
{
    _hash = 0; // Mark for rehashing.
    _serviceData = list;
}
 */

/**
 * @details
 * Sets the required stream data string list.
void DataSpec::setStreamData(const QSet<QString>& list)
{
    _hash = 0; // Mark for rehashing.
    _streamData = list;
}
 */

/**
 * @details
 * Tests if this requirements object is the same as the other one.
 */
bool DataSpec::operator==(const DataSpec& d) const
{
    return hash() == d.hash();
}

/**
 * @details
 * Tests if this requirements object is not the same as the other one.
 */
bool DataSpec::operator!=(const DataSpec& d) const
{
    return !(*this == d);
}

/**
 * @details
 * Merges the given data requirements with those already known to this
 * class, removing any duplicates.
 */
DataSpec& DataSpec::operator+=(const DataSpec& d)
{
    _streamData.unite(d.streamData());
    _serviceData.unite(d.serviceData());
    _hash = 0; // Mark for rehashing.
    return *this;
}

/**
 * @details
 * Merges the two sets of requirements together and returns a new instance
 * with the result.
 */
const DataSpec DataSpec::operator+(const DataSpec& d) const
{
    return DataSpec(*this) += d;
}

/**
 * @details
 * Test if the DataSpec object is compatible with a hash of data blobs.
 */
bool operator==(const DataSpec& r, const QHash<QString, DataBlob*>& hash)
{
    return r.isCompatible(hash);
}

int DataSpec::size() const
{
    return _streamData.size() + _serviceData.size();
}

void DataSpec::addAdapterTypes( const QHash<QString, QString>& types ) {
    foreach( const QString& s, types ) {
        _adapterTypes.insert(s, types[s] );
    }
}

/**
 * @details
 * Provides a hash value for the DataSpec object for use with QHash.
 */
uint qHash(const DataSpec& key)
{
    return key.hash();
}

} // namespace pelican
