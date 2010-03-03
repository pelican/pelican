#ifndef DATA_H
#define DATA_H

#include <cstring>
#include <QObject>

/**
 * @file Data.h
 */

namespace pelican {

/**
 * @class Data
 *  
 * @brief
 *    Primary interface to access Chunks of data in the server
 * @details
 *    This class takes care of locking etc.
 */

class Data
{

    public:
        Data(const QString& name="", void* data=0, size_t size=0);
        ~Data();

        /// return the size of the stored data
        size_t size() const;

        /// sets the size of the stored data
        void setSize(size_t);

        /// returns a pointer to the beginning of the memory block
        void* operator*();

        const QString& name() const { return _name; }
        void setName(const QString& name );

        /// return the data id 
        QString id() const { return _id; }

        /// sets the id 
        void setId(const QString& id) { _id = id; }

        // returns true if any data exists
        bool isValid() const;

    private:
        Data(const Data&); // disallow the copy constructor

    private:
        QString _name;
        QString _id;
        void* _data;
        size_t _size;
};

} // namespace pelican
#endif // DATA_H 
