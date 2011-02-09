#ifndef DATABLOB_H
#define DATABLOB_H

/**
 * @file DataBlob.h
 */

#include "pelican/utility/FactoryRegistrar.h"

#include <QtCore/QString>
#include <QtCore/QSysInfo>
#include <complex>

class QIODevice;

namespace pelican {

/**
 * This macro is used to register the named data blob type.
 */
#define PELICAN_DECLARE_DATABLOB(type) PELICAN_DECLARE(DataBlob, type)

/**
 * @ingroup c_data
 *
 * @class DataBlob
 *
 * @brief
 * Base class for all Pelican data blobs.
 *
 * @details
 * This is the base class used for all Pelican data blobs required by
 * pipeline modules.
 */

class DataBlob
{
    public:
        /// Data blob constructor.
        PELICAN_CONSTRUCT_TYPES_EMPTY
        DataBlob(const QString& type);

        /// Data blob destructor.
        virtual ~DataBlob() {}

    public:
        /// Returns the type of DataBlob (should be the class name).
        const QString& type() const {return _type;}

        /// Set the version ID of the DataBlob.
        void setVersion(const QString& v) { _version = v; }

        /// Returns the version of the DataBlob.
        const QString& version() const { return _version; }

    public:
        /// Serialise the DataBlob into the QIODevice.
        virtual void serialise(QIODevice&) const;

        /// Returns the number of serialised bytes.
        virtual quint64 serialisedBytes() const;

        /// Deserialises the DataBlob from the QIODevice.
        virtual void deserialise(QIODevice&, QSysInfo::Endian endianness);

    private:
        QString _version;
        QString _type;
};

} // namespace pelican
#endif // DATABLOB_H
