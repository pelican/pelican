#ifndef DATA_H
#define DATA_H

#include <cstring>
#include <QMutex>
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

class Data : public QObject
{

    Q_OBJECT

    public:
        Data(void* data=0, size_t size=0, QObject* parent=0);
        ~Data();
        /// return the size of the stored data
        size_t size() const;
        /// sets the size of the stored data
        void setSize(size_t);
        /// returns a pointer to the beginning of the memory block
        void* operator*();
        /// marks the data as locked (increases count on unlimited semaphore)
        void lock();
        /// marks the data as unlocked (decreases count on semaphore)
        void unlock();
        /// indicates if there is any usable data
        bool isValid() const;

    signals:
        void unlocked(Data*);

    protected:
        QMutex _mutex;

    private:
        Data(const Data&); // disallow the copy constructor

    private:
        void* _data;
        size_t _size;
        int _lock;
};

} // namespace pelican
#endif // DATA_H 
