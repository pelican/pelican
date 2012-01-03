#ifndef LOCKINGCIRCULARBUFFER_H
#define LOCKINGCIRCULARBUFFER_H
#include "CircularBufferIterator.hpp"
#include <QtCore/QList>


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
             _lock = new CircularBufferIterator<T>(dataBuffer);
             _current = new CircularBufferIterator<T>(dataBuffer);
        }
        ~LockingCircularBuffer() { delete _lock; delete _current; };

        /// return the next free item, this will BLOCK until there are free
        //  elements.
        T next() {
             const T& next = _current->peekNext();
             while( *(*_lock) == next ) {}
             return _current->next();
        };

        /// move the lock pointer to the next position
        //  and return a CircularBufferIterator starting at the lock position
        const CircularBufferIterator<T>& shiftLock() {
             _lock->next();
             return *_lock;
        }

        /// return the value pointed to by the current lock
        T& current() const { return *(*_current); }

    private:
        CircularBufferIterator<T>* _lock;
        CircularBufferIterator<T>* _current;
};

} // namespace pelican
#endif // LOCKINGCIRCULARBUFFER_H 
