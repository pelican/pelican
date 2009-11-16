/*
 * Adapted from:
 * "C++ in Action"
 * http://www.relisoft.com/book/tech/9new.html
 */

#include "memoryTracer.h"
#include <sstream>
#include <iostream>
#include <new>

#if !defined NDEBUG

bool Tracer::ready = false;

void* operator new(std::size_t size, char const * file, int line)
{
    void * p = malloc (size);
    //std::cout << "new() :" << file << " line: " << line << std::endl;
    if (Tracer::ready)
        boost::details::pool::singleton_default<Tracer>::instance().add(p, file, line);
    return p;
}

void operator delete(void * p, char const * file, int line)
{
    if (Tracer::ready)
        boost::details::pool::singleton_default<Tracer>::instance().remove (p);
    //std::cout << "delete() :" << file << " line: " << line << std::endl;
    free(p);
}

void* operator new(std::size_t size) throw (std::bad_alloc)
{
    void * p = malloc (size);
    if (Tracer::ready)
        boost::details::pool::singleton_default<Tracer>::instance().add (p, "?", 0);
    return p;
}

void operator delete(void* p) throw()
{
    if (Tracer::ready)
        boost::details::pool::singleton_default<Tracer>::instance().remove(p);
    free(p);
}

Tracer::Tracer () 
: _lockCount (0) 
{
    ready = true;
}

Tracer::~Tracer ()
{
    ready = false;
    dump ();
}

bool Tracer::entry( void* p ) const {
    return _map.find(p) != _map.end();
}

void Tracer::add (void* p, char const * file, int line)
{
    if (_lockCount > 0)
        return;
    Tracer::Lock lock (*this);
    _map [p] = Entry (file, line);
}

void Tracer::remove (void * p)
{
    if (_lockCount > 0)
        return;

    Tracer::Lock lock (*this);

    iterator it = _map.find (p);
    if (it != _map.end ())
    {
        char const * file = it->second.file ();
        int line = it->second.line ();
        _map.erase (it);
    }
}

void Tracer::dump ()
{
    if (_map.size () != 0)
    {
        std::cerr << ("*** Memory leak(s):\n");
        for (iterator it = _map.begin (); it != _map.end (); ++it)
        {
            char const * file = it->second.file ();
            int line = it->second.line ();
            void* addr = it->first;
            std::stringstream out;
            //out << "0x" << std::hex << addr << ": " 
            //    << file << ", line " << std::dec << line << std::endl;
            out << "0x" << std::hex << addr << ": "
                << file << ", line " << std::dec << line << std::endl;
            std::cerr << (out.str().c_str ());
        }
        std::cerr << std::endl;
    }
}

#endif
