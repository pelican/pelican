#include <new>

void* operator new(std::size_t size, char const * file, int line);
void  operator delete(void * p, char const * file, int line);
extern void* operator new(std::size_t size) throw (std::bad_alloc);
extern void  operator delete(void * p) throw();
