/*
 * Adapted from:
 * "C++ in Action"
 * http://www.relisoft.com/book/tech/9new.html
 */
#if !defined DEBUGNEW_H
#define DEBUGNEW_H
#pragma warning (disable:4786)
#include <cstdlib>
#include <string>
#include <QMutex>
#include <new>
#include <boost/pool/detail/singleton.hpp>

void* operator new(std::size_t size, char const * file, int line);
void  operator delete(void * p, char const * file, int line);
extern void* operator new(std::size_t size) throw (std::bad_alloc);
extern void  operator delete(void * p) throw();

#include <map>

class Tracer
{
private:
    class Entry
    {
        public:
            Entry (char const * file, int line)
                : _file(file), _line (line) {}
            Entry ()
                :  _line (0) {}
            char const * file() const { return _file.c_str(); }
            int line() const { return _line; }
        private:
            std::string _file;
            int _line;
    };
    typedef std::map<void *, Entry>::iterator iterator;
public:
    Tracer();
    ~Tracer();
    void add(void * p, char const * file, int line);
    void remove(void * p);
    void dump();
    bool entry(void* p) const;

    static bool ready;
private:
    QMutex _mutex;
    std::map<void*, Entry> _map;
};


#endif
