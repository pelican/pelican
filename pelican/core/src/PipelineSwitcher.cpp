#include "PipelineSwitcher.h"
#include <QtCore/QString>

namespace pelican {


/**
 * @details Constructs a PipelineSwitcher object.
 */
PipelineSwitcher::PipelineSwitcher( bool cycle )
    : _cycle(cycle), _current(-1)
{
}

/**
 * @details Destroys the PipelineSwitcher object.
 */
PipelineSwitcher::~PipelineSwitcher()
{
}

void PipelineSwitcher::addPipeline(AbstractPipeline* pipe)
{
     _pipelines.append(pipe);
}

AbstractPipeline* PipelineSwitcher::next()
{
     int size = _pipelines.size();
     if( size == 0 )
         throw(QString("PipelineSwitcher: No pipelines defined"));
     if( _cycle )
         return _pipelines[++_current%size];
     if( ++_current < size )
        return _pipelines[_current];
     return 0;
}

} // namespace pelican
