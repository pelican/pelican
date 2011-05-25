#ifndef PELICANTIMER_H
#define PELICANTIMER_H

#include <QtCore/QTime>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QString>
#include <vector>
#include <map>

/**
 * @file PelicanTimeRecorder.h
 */

namespace pelican {

/**
 * @class PelicanTimeRecorder
 * 
 * @brief
 *    A time check reporting class
 * @details
 *    Use for profiling your code in integration tests etc.
 */
class PelicanTimeRecorder
{
    public:
        /// PelicanTimeRecorder constructor.
        PelicanTimeRecorder( int reportInterval = 0);

        /// PelicanTimeRecorder destructor.
        ~PelicanTimeRecorder();

        /// return the instance of the PelicanTimeRecorder
        static PelicanTimeRecorder* instance();

        /// set the number of calls between reports
        void setReportInterval(int rl);

        /// mark the time since the last tick call
        //  and associate this with the given reporting id
        void tick(const QString& id);

        /// start/reset the timer
        void start();

        /// output the report data
        //  foe each reporting id the tick times will
        //  be calculated and printed to stdout
        void report() const;

    private:
        typedef QMap<QString,std::vector<unsigned int> > T_Map;
        mutable QMutex _mutex;
        static PelicanTimeRecorder* _globalTimer;
        int _reportInterval;
        int _count;
        QTime _timer;
        T_Map _ticks;
};

extern PelicanTimeRecorder* __globalTimeRecorder;

} // namespace pelican

#endif // PELICANTIMER_H 
