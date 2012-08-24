#include "DataRequirements.h"
#include <QtCore/QStringList>


namespace pelican {


/**
 *@details DataRequirements 
 */
DataRequirements::DataRequirements()
    : _hash(0)
{
}

/**
 *@details
 */
DataRequirements::~DataRequirements()
{
}

void DataRequirements::addRequired( const QString& stream ) {
    _hash = 0;
    _requiredData.insert(stream);
}

void DataRequirements::addRequired( const QSet<QString>& streams ) {
    _hash = 0;
    _requiredData.unite( streams );
}

void DataRequirements::addOptional( const QString& stream ) {
    _hash = 0;
    _optionalData.insert(stream);
}

void DataRequirements::addOptional( const QSet<QString>& streams ) {
    _hash = 0;
    _optionalData.unite( streams );
}

bool DataRequirements::contains(const QString& stream) const {
     return _requiredData.contains(stream) || _optionalData.contains(stream);
}

void DataRequirements::clear() {
    _hash = 0;
    _requiredData.clear();
    _optionalData.clear();
}

QSet<QString> DataRequirements::allStreams() const {
#ifdef BROKEN_QT_SET_HEADER
    // The Qt header misses a const declaration so we have to make
    // a double copy
    QSet<QString> temp = _requiredData;
    return temp + _optionalData;
#else
    return _requiredData + _optionalData;
#endif
}

bool DataRequirements::operator==(const DataRequirements& d) const
{
    return hash() == d.hash();
}

/**
 * @details
 * Tests if this requirements object is not the same as the other one.
 */
bool DataRequirements::operator!=(const DataRequirements& d) const
{
    return !(*this == d);
}

/**
 * @details
 * Computes a hash of the requirements lists for use as a key with QHash.
 */
uint DataRequirements::hash() const
{
    if (_hash == 0) {
        /* Sort the requirements lists before recomputing the hash */
        QStringList requiredCopy = _requiredData.values();
        QStringList optionalCopy = _optionalData.values();
        qSort(requiredCopy.begin(), requiredCopy.end() );
        qSort(optionalCopy.begin(), optionalCopy.end() );
        _hash = qHash(requiredCopy.join(QString()) + optionalCopy.join(QString()));
    }
    return _hash;
}

DataRequirements& DataRequirements::operator+=(const DataRequirements& d)
{
    _requiredData.unite(d._requiredData);
    _optionalData.unite(d._optionalData);
    _hash = 0; // Mark for rehashing.
    return *this;
}

void DataRequirements::remove( const QList<QString>& list ) {
    _requiredData.subtract(list.toSet());
    _optionalData.subtract(list.toSet());
}

int DataRequirements::size() const {
    return _requiredData.size() + _optionalData.size();
}

/**
 * @details
 * Provides a hash value for the DataRequirements object for use with QHash.
 */
uint qHash(const DataRequirements& key)
{
    return key.hash();
}

/// Test for compatibility with a data blob hash.
bool operator==(const DataRequirements& d, const QList<QString>& h) {
    DataRequirements temp(d);
    temp.remove(h);
    return temp.size() == 0;
}

} // namespace pelican
