#include "PelicanTimeRecorder.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <QMutexLocker>
#include "pelican/utility/memCheck.h"

namespace pelican {

PelicanTimeRecorder* PelicanTimeRecorder::_globalTimer = 0;

/**
 * @details Constructs a PelicanTimeRecorder object.
 */
PelicanTimeRecorder::PelicanTimeRecorder( int reportInterval )
    : _reportInterval(reportInterval)
{
    _count = 0;
}

/**
 * @details Destroys the PelicanTimeRecorder object.
 */
PelicanTimeRecorder::~PelicanTimeRecorder()
{
}

void PelicanTimeRecorder::setReportInterval(int rl)
{
    _reportInterval = rl;
}

PelicanTimeRecorder* PelicanTimeRecorder::instance() {
    if( !  _globalTimer )
        _globalTimer = new PelicanTimeRecorder;
    return _globalTimer;
}

void PelicanTimeRecorder::start()
{
    QMutexLocker loc(&_mutex);
    _count = 0;
    _last = _timer.elapsed();
}

void PelicanTimeRecorder::tick(const QString& id)
{
    int ms = _timer.elapsed() - _last;
    _ticks[id].push_back( ms );
    if( ++_count%_reportInterval == 0)  report();
    _last = _timer.elapsed();
}

void PelicanTimeRecorder::report() const
{
    foreach(const QString& tag, _ticks.keys() ) {
        int max = *(std::max_element(_ticks[tag].begin(), _ticks[tag].end() ));
        int min = *(std::min_element(_ticks[tag].begin(), _ticks[tag].end() ));
        std::cout << tag.toStdString()
                  << ": mean=" << std::accumulate( _ticks[tag].begin(), _ticks[tag].end(), 0 )
                                             / _ticks[tag].size();
        std::cout << ": max=" << max << ": min=" << min
                  << " (ms) " << _ticks[tag].size() 
                  << " measurements" << std::endl;
    }
}

} // namespace pelican
