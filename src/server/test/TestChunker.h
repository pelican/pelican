#ifndef TESTCHUNKER_H
#define TESTCHUNKER_H

#include "AbstractChunker.h"

/**
 * @file TestChunker.h
 */

namespace pelican {
class DataManager;

/**
 * @class TestChunker
 *
 * @brief
 *   Implemnetation of an AbstractChunker to monitor
 *   calling
 * @details
 * 
 */

class TestChunker : public AbstractChunker
{
    public:
        TestChunker(DataManager* dm, bool badSocket=false);
        ~TestChunker();
        virtual QAbstractSocket* newSocket() const;
        virtual void next(QAbstractSocket*);

        /// return the number of times that the next method has been called
        //  will also reset the counter
        unsigned int nextCalled();

    private:
        bool _badSocket;
        unsigned int _nextCount;
};

} // namespace pelican
#endif // TESTCHUNKER_H 
