#include "TestChunker.h"

#include "utility/memCheck.h"

namespace pelican {


// class TestChunker 
TestChunker::TestChunker( DataManager* dm, bool badSocket )
    : AbstractChunker(dm), _badSocket(badSocket)
{
    _nextCount = 0;
}

TestChunker::~TestChunker()
{
}

QAbstractSocket* TestChunker::newSocket() const
{
    if(_badSocket)
        return 0;
    return AbstractChunker::newSocket();
}

void TestChunker::next(QAbstractSocket*) 
{
    ++_nextCount;
}

unsigned int TestChunker::nextCalled()
{
    unsigned int n = _nextCount;
    _nextCount = 0;
    return n;
}

} // namespace pelican
