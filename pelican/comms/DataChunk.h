#ifndef DATA_CHUNK_H
#define DATA_CHUNK_H

/**
 * @file DataChunk.h
 */

#include <QtCore/QObject>

#include <cstring>
#include <typeinfo>
#include <cstdlib>

class QByteArray;

namespace pelican {

/**
 * @ingroup c_comms
 *
 * @class DataChunk
 *
 * @brief
 * Primary interface to access chunks of data in the server.
 *
 * @details
 * Holds the pointer to the memory location for the data chunk,
 * the size of the valid data contained within the data chunk, and the name
 * and the version of the data.
 *
 * The stream data object inherits this class.
 */

class DataChunk
{
    public:
        /// Constructs a new Data object.
        DataChunk(const QString& name = "", void* data = 0, size_t size = 0)
        : _name(name), _data(data), _size(size) {}

        /// Constructs an empty Data object.
        DataChunk(const QString& name, const QString& id, size_t size = 0)
        : _name(name), _id(id), _data(0), _size(size) {}

        /// Constructs a new Data object from the given byte array.
        DataChunk(const QString& name, const QString& id, QByteArray& ba)
        : _name(name), _id(id)
        {
            _data = ba.data();
            _size = ba.size();
        }

        /// Destroys the Data object.
        virtual ~DataChunk() {}

        /// Returns the size of the stored data.
        size_t size() const { return _size; }

        /// Sets the size of the stored data.
        void setSize(size_t s) { _size = s; }

        /// Returns a pointer to the beginning of the memory block.
        void* ptr() { return _data; }

        /// Returns a pointer to the beginning of the memory block.
        const void* ptr() const { return _data; }

        /// Returns a pointer to the beginning of the memory block.
        void* data() { return _data; }

        /// Returns a pointer to the beginning of the memory block.
        const void* data() const { return _data; }

        /// Returns the name of the data.
        const QString& name() const { return _name; }

        /// Sets the name of the data.
        void setName(const QString& name) { _name = name; }

        /// Returns the data ID.
        QString id() const { return _id; }

        /// Sets the ID.
        void setId(const QString& id) { _id = id; }

        /// Returns true if any data exists.
        virtual bool isValid() const
        { return !( _data == 0 || _size == 0); }

        /// Returns true if the object contains identical meta-data.
        bool operator==(const DataChunk& d) const
        { return bool(_name == d.name() && _size == d._size && _id == d._id); }

    private:
        QString _name; // The name of the object.
        QString _id;   // The ID of the object.
        void* _data;   // Pointer to the data.
        size_t _size;  // Size of the data in bytes.

    private:
        DataChunk(const DataChunk&); // Disallow the copy constructor.
};

} // namespace pelican
#endif // DATA_CHUNK_H
