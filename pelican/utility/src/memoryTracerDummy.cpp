#include "memoryCheckInterface.h"
#include <new>

void* operator new(std::size_t size, char const * /*file*/, int /*line*/)
{
    return ::operator new(size);
}

void operator delete(void * p, char const * /*file*/, int /*line*/)
{
    ::operator delete(p);
}
