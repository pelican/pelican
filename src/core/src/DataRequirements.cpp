#include "DataRequirements.h"
#include <QHash>

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
    _streamData.clear();
    _streamData.append(string);
}

/**
 * @details
 * Sets the required service data string list.
 */
void DataRequirements::setServiceData(const QString& string)
{
    _serviceData.clear();
    _serviceData.append(string);
}

/**
 * @details
 * Sets the required stream data string list.
 */
void DataRequirements::setStreamData(const QStringList& list)
{
    _streamData = list;
}

/**
 * @details
 * Sets the required service data string list.
 */
void DataRequirements::setServiceData(const QStringList& list)
{
    _serviceData = list;
}


/**
 * @details
 */
uint DataRequirements::hash() const
{
    if (_hash == 0) {
        QString stringKey;
        foreach (QString string, _streamData) {
            stringKey += string;
        }
        foreach (QString string, _serviceData) {
            stringKey += string;
        }
        _hash = qHash(stringKey);
    }
    return _hash;
}

/**
 * @details
 * Operator + for DataRequirements. Merges the given data
 * requirements with those already known to this class.
 */
DataRequirements& DataRequirements::operator+(const DataRequirements& d)
{
    
    return *this;
}

/**
 * @details
 * Operator == for DataRequirements.
 */
bool DataRequirements::operator==(const DataRequirements& d) const
{
    return (hash() == d.hash());
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
