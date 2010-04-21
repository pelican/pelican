/* 
 * Debugging Header
 * Override new/delete operators to provide useful memory leak info
 */
#ifndef NDEBUG
#ifdef USE_MEMORY_TRACKER
#ifndef MEMCHECK_NEW_H
#define MEMCHECK_NEW_H 1
#include "memoryCheckInterface.h"

#include <iostream>
#define new new(__FILE__, __LINE__)

#endif // MEMCHECK_NEW_H
#endif // USE_MEMORY_TRACKER
#endif // DEBUG
