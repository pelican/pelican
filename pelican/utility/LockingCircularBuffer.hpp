#ifndef LOCKINGCIRCULARBUFFER_H
#define LOCKINGCIRCULARBUFFER_H
#include "CircularBufferIterator.hpp"
#include <QtCore/QList>
#include <QtCore/QWaitCondition>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>


/**
 * @file LockingCircularBuffer.h
 */

namespace pelican {

/**
 * @class LockingCircularBuffer
 *  
 * @brief
 *   A convenience class to manage two pointers on the same circular buffer
 *   Two pointers are defined, lthe lock pointer and the current pointer.
 *   The current pointer will not be allowed to move past the position 
 *   of the lock pointer.
 * @details
 * 
 */

template<typename T>
class LockingCircularBuffer
{
    public:
        LockingCircularBuffer( QList<T>* dataBuffer ) {
             _lock = 0;
             _current = new CircularBufferIterator<T>(dataBuffer);
        }
        ~LockingCircularBuffer() { delete _lock; delete _current; };

        /// return the next free item, this will BLOCK until there are free
        //  elements.
        T next() {
             QMutexLocker lock(&_mutex);
             const T& next = _current->peekNext();
             while( _lock && *(*_lock) == next ) {
                _waitCondition.wait(&_mutex);
             }
             return _current->next();
        };

        /// move the lock pointer to the next position
        //  and return a CircularBufferIterator starting at the lock position
        const CircularBufferIterator<T>& shiftLock() {
             QMutexLocker lock(&_mutex);
             if( ! _lock ) { 
                 _lock = new CircularBufferIterator<T>(_current->buffer());
             }
             _lock->next();
             _waitCondition.wakeOne();
             return *_lock;
        }

        /// return the value pointed to by the current lock
        T& current() const { return *(*_current); }

        /// returns a pointer to the underlying data buffer
        QList<T>* rawBuffer() { return _current->buffer(); }

    private:
        QWaitCondition _waitCondition;
        QMutex _mutex;
        CircularBufferIterator<T>* _lock;
        CircularBufferIterator<T>* _current;
};

} // namespace pelican
#endif // LOCKINGCIRCULARBUFFER_H 
