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
#include "memoryCheckInterface.h"

#include <map>

class Tracer
{
private:
    class Entry
    {
        public:
            Entry(char const* file, int line) : _file(file), _line(line) {}
            Entry() : _line (0) {}
            char const* file() const {return _file;}
            int line() const {return _line;}
        private:
            const char* _file;
            int _line;
    };

    class Lock
    {
        public:
            Lock(Tracer& tracer) : _tracer(tracer) {_tracer.lock();}
            ~Lock() {_tracer.unlock();}
        private:
            Tracer& _tracer;
    };

    typedef std::map<void *, Entry>::iterator iterator;
    friend class Lock;

public:
    Tracer();
    ~Tracer();
    void add(void* p, char const* file, int line);
    void remove(void* p);
    void dump();

    static bool ready;

private:
    void lock() {++_lockCount;}
    void unlock() {--_lockCount;}

private:
    std::map<void*, Entry> _map;
    int _lockCount;
};

#endif
