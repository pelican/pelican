#ifndef PIPELINESWITCHER_H
#define PIPELINESWITCHER_H
#include <QtCore/QList>


/**
 * @file PipelineSwitcher.h
 */

namespace pelican {
class AbstractPipeline;

/**
 * @class PipelineSwitcher
 *
 * @brief
 *     Provides a way of switching between alternative pipelines
 * @details
 *     This is a container class of suitable pipelines that should
 *     be cycled through. The pipelines will be iterated through in the
 *     order added. Each new pipeline will be installed whenever the
 *     previous one is disabled
 *
 */
class PipelineSwitcher
{
    public:
        /// PipelineSwitcher constructor.
        PipelineSwitcher(bool cycle=true);

        /// PipelineSwitcher destructor.
        ~PipelineSwitcher();

        /// add the next pipeline to be run
        void addPipeline(AbstractPipeline*);

        /// return the list of all pipelines in the switcher
        const QList<AbstractPipeline*>& pipelines() const { return _pipelines; }

        /// return the next pipeline to be run
        AbstractPipeline* next();

    private:
        bool _cycle;
        int _current;
        QList<AbstractPipeline*> _pipelines;
};

} // namespace pelican

#endif // PIPELINESWITCHER_H
