#include "PelicanTimeRecorder.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <QtCore/QMutexLocker>

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
    _ticks.clear();
    _timer.start();
}

void PelicanTimeRecorder::tick(const QString& id)
{
    _ticks[id].push_back( _timer.elapsed() );
    if( _reportInterval && (++_count)%_reportInterval == 0)  report();
    _timer.restart();
}

void PelicanTimeRecorder::report() const
{
    foreach(const QString& tag, _ticks.keys() ) {
        if( _ticks[tag].size() > 0 ) {
            std::vector<unsigned int> temp = _ticks[tag];
            int max = *(std::max_element(temp.begin(), temp.end() ));
            int min = *(std::min_element(temp.begin(), temp.end() ));
            std::cout << tag.toStdString()
                << ": mean=" << std::accumulate( temp.begin(), temp.end(), 0 )
                / _ticks[tag].size();
            std::cout << ": max=" << max << ": min=" << min
                << " (ms) " << _ticks[tag].size()
                << " measurements" << std::endl;
        }
    }
}

} // namespace pelican
