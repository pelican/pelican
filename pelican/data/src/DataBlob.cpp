#include "pelican/data/DataBlob.h"
#include <QtCore/QIODevice>

namespace pelican {

/**
 * @details
 * Constructs a new data blob, setting the type-name.
 *
 * @param[in] type The name of the data blob derived class.
 */
DataBlob::DataBlob(const QString& type) : _type(type)
{
}

/**
 * @details
 * Serialises the DataBlob into the QIODevice.
 * This method should be re-implemented in a derived class if needed,
 * since the default implementation will throw an exception of type QString.
 */
void DataBlob::serialise(QIODevice&) const
{
    throw QString("DataBlob: This object's ('%1') serialise method "
            "is undefined.").arg(type());
}

/**
 * @details
 * This method must return the number of bytes saved to the QIODevice
 * for the serialised data blob when calling the serialise() method.
 */
quint64 DataBlob::serialisedBytes() const
{
    throw QString("DataBlob: This object's ('%1') serialisedBytes method "
            "is undefined.").arg(type());
}

/**
 * @details
 * Deserialises the data blob.
 * This method should be re-implemented in a derived class if needed,
 * since the default implementation will throw an exception of type QString.
 */
void DataBlob::deserialise(QIODevice&, QSysInfo::Endian)
{
    throw QString("DataBlob: This object's ('%1') deserialise method "
            "is undefined.").arg(type());
}

} // namespace pelican
