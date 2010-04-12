#ifndef LOFARTYPES_H_
#define LOFARTYPES_H_

#include <complex>
#include <cmath>
#include <cstdio>
#include <boost/cstdint.hpp>

namespace pelican {

namespace TYPES {

// Convenience shortcuts.
typedef unsigned char        uchar;
typedef unsigned short       ushort;
typedef unsigned int         uint;
typedef unsigned long        ulong;
typedef long long            longlong;
typedef unsigned long long   ulonglong;
typedef long double          ldouble;

// Fixed data sizes.
typedef signed char         int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;

typedef std::complex<int8>   i8complex;
typedef std::complex<int16>  i16complex;

class i4complex
{
    public:
        i4complex() {}

        i4complex(double real, double imag) {
            value = ((int) rint(real - .5) & 0xF) | (((int) rint(imag - .5) & 0xF) << 4);
        }

        double real() const {
            return ((signed char) (value << 4) >> 4) + .5; // extend sign
        }

        double imag() const {
            return (value >> 4) + .5;
        }

        i4complex conj() const {
            return i4complex(value ^ 0xF0);
        }

    private:
        i4complex(unsigned char value)
        : value(value) {}

        // do not use bitfields for the real and imaginary parts, since the
        // allocation order is not portable between different compilers
        signed char value;
};

// Functions operating on int4 complex numbers.
// <group>
inline double real(TYPES::i4complex v)
{ return v.real(); }
inline double imag(TYPES::i4complex v)
{ return v.imag(); }
inline TYPES::i4complex conj(TYPES::i4complex x)
{ return x.conj(); }
// </group>

// Functions to make complex numbers.
// <group>
inline TYPES::i4complex makei4complex(double real, double imag)
{ return TYPES::i4complex(real, imag); }
inline TYPES::i8complex makei8complex (TYPES::int8 re, TYPES::int8 im)
{ return TYPES::i8complex(re, im); }
inline TYPES::i16complex makei16complex (TYPES::int16 re, TYPES::int16 im)
{ return TYPES::i16complex(re, im); }
// </group>

// Functions operating on int8 complex numbers.
// <group>
inline TYPES::int8 real (TYPES::i8complex x)
{ return x.real(); }
inline TYPES::int8 imag (TYPES::i8complex x)
{ return x.imag(); }
inline TYPES::i8complex conj (TYPES::i8complex x)
{ return conj(x); }
// </group>

// Functions operating on int16 complex numbers.
// <group>
inline TYPES::int16 real (TYPES::i16complex x)
{ return x.real(); }
inline TYPES::int16 imag (TYPES::i16complex x)
{ return x.imag(); }
inline TYPES::i16complex conj (TYPES::i16complex x)
{ return conj(x); }
// </group>




class TimeStamp {

    public:
        TimeStamp(int64 time = 0);
        TimeStamp(unsigned seqId, unsigned blockId);

        TimeStamp       &setStamp(unsigned seqId, unsigned blockId);
        unsigned        getSeqId() const;
        unsigned        getBlockId() const;

        void            setStationClockSpeed(unsigned speed);

        template <typename T> TimeStamp &operator += (T increment);
        template <typename T> TimeStamp &operator -= (T decrement);
        TimeStamp  operator ++ (int); // postfix
        TimeStamp &operator ++ ();    // prefix
        TimeStamp  operator -- (int);
        TimeStamp &operator -- ();

        template <typename T> TimeStamp  operator +  (T) const;
        template <typename T> TimeStamp  operator -  (T) const;
        int64      operator -  (const TimeStamp &) const;

        bool       operator >  (const TimeStamp &) const;
        bool       operator <  (const TimeStamp &) const;
        bool       operator >= (const TimeStamp &) const;
        bool       operator <= (const TimeStamp &) const;
        bool       operator == (const TimeStamp &) const;
        bool       operator != (const TimeStamp &) const;

        operator int64 () const;
        operator struct timespec () const;

        friend std::ostream &operator << (std::ostream &os, const TimeStamp &ss);
        int64           itsTime;

    protected:

        unsigned        clockSpeed;

};

// unsigned TimeStamp::clockSpeed = 20000000;

inline TimeStamp::TimeStamp(int64 time)
{   itsTime = time;   }

inline TimeStamp::TimeStamp(unsigned seqId, unsigned blockId)
{
#ifdef EVEN_SECOND_HAS_MORE_SAMPLES
    itsTime = ((int64) seqId * clockSpeed + 512) / 1024 + blockId;
#else
    itsTime = ((int64) seqId * clockSpeed) / 1024 + blockId;
#endif
}

inline TimeStamp &TimeStamp::setStamp(unsigned seqId, unsigned blockId)
{
#ifdef EVEN_SECOND_HAS_MORE_SAMPLES
    itsTime = ((int64) seqId * clockSpeed + 512) / 1024 + blockId;
#else
    itsTime = ((int64) seqId * clockSpeed) / 1024 + blockId;
#endif
    return *this;
}

inline unsigned TimeStamp::getSeqId() const
{
#ifdef EVEN_SECOND_HAS_MORE_SAMPLES
    return (unsigned) (1024 * itsTime / clockSpeed);
#else
    return (unsigned) ((1024 * itsTime + 512) / clockSpeed);
#endif
}

inline unsigned TimeStamp::getBlockId() const
{
#ifdef EVEN_SECOND_HAS_MORE_SAMPLES
    return (unsigned) (1024 * itsTime % clockSpeed / 1024);
#else
    return (unsigned) ((1024 * itsTime + 512) % clockSpeed / 1024);
#endif
}

inline void TimeStamp::setStationClockSpeed(unsigned speed)
{
    printf("Using a %f MHz clock", speed/1e6);
    clockSpeed = speed;
}

template <typename T> inline TimeStamp &TimeStamp::operator += (T increment)
        {
    itsTime += increment;
    return *this;
        }

template <typename T> inline TimeStamp &TimeStamp::operator -= (T decrement)
        {
    itsTime -= decrement;
    return *this;
        }

inline TimeStamp &TimeStamp::operator ++ ()
{
    ++ itsTime;
    return *this;
}

inline TimeStamp TimeStamp::operator ++ (int)
{
    TimeStamp tmp = *this;
    ++ itsTime;
    return tmp;
}

inline TimeStamp &TimeStamp::operator -- ()
{
    -- itsTime;
    return *this;
}

inline TimeStamp TimeStamp::operator -- (int)
{
    TimeStamp tmp = *this;
    -- itsTime;
    return tmp;
}

template <typename T> inline TimeStamp TimeStamp::operator + (T increment) const
{   return TimeStamp(itsTime + increment);    }

template <typename T> inline TimeStamp TimeStamp::operator - (T decrement) const
{   return TimeStamp(itsTime - decrement);    }

inline int64 TimeStamp::operator - (const TimeStamp &other) const
{    return itsTime - other.itsTime;    }

inline TimeStamp::operator int64 () const
{   return itsTime;   }

inline TimeStamp::operator struct timespec () const
{
        int64       ns = (int64) (itsTime * 1024 * 1e9 / clockSpeed);
        struct timespec ts;

        ts.tv_sec  = ns / 1000000000ULL;
        ts.tv_nsec = ns % 1000000000ULL;

        return ts;
}

inline bool TimeStamp::operator > (const TimeStamp &other) const
{   return itsTime > other.itsTime;     }

inline bool TimeStamp::operator >= (const TimeStamp &other) const
{   return itsTime >= other.itsTime;    }

inline bool TimeStamp::operator < (const TimeStamp &other) const
{   return itsTime < other.itsTime;     }

inline bool TimeStamp::operator <= (const TimeStamp &other) const
{   return itsTime <= other.itsTime;    }

inline bool TimeStamp::operator == (const TimeStamp &other) const
        {   return itsTime == other.itsTime;    }

inline bool TimeStamp::operator != (const TimeStamp &other) const
        {   return itsTime != other.itsTime;    }

} // namespace types

} // namespace pelican

#endif /* LOFARTYPES_H_ */
