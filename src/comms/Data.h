#ifndef DATA_H
#define DATA_H

#include <cstring>
#include <QObject>
class QByteArray;

/**
 * @file Data.h
 */

namespace pelican {

/**
 * @class Data
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
class Data
{
    public:
        Data(const QString& name="", void* data=0, size_t size=0);
        Data(const QString& name, const QString& id, size_t size=0);
        Data(const QString& name, const QString& id, QByteArray& );
        virtual ~Data() {}

        /// Returns the size of the stored data.
        size_t size() const {return _size;}

        /// Sets the size of the stored data.
        void setSize(size_t s) {_size = s;}

        /// Returns a pointer to the beginning of the memory block.
        void* operator*() {return _data;}

        /// Returns a pointer to the beginning of the memory block.
        const void* operator*() const {return _data;}

        /// Returns the name of the data.
        const QString& name() const { return _name; }

        /// Sets the name of the data.
        void setName(const QString& name ) {_name = name;}

        /// Returns the data ID.
        QString id() const { return _id; }

        /// Sets the ID.
        void setId(const QString& id) { _id = id; }

        /// Returns true if any data exists.
        virtual bool isValid() const {return !( _data == 0 || _size == 0);}

        /// Returns true if the object contains identical meta-data.
        bool operator==(const Data&) const;

    private:
        Data(const Data&); // Disallow the copy constructor.

    private:
        QString _name;
        QString _id;
        void* _data;
        size_t _size;
};

} // namespace pelican

#endif // DATA_H 
