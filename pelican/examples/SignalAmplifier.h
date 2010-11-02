#ifndef SIGNALAMPLIFIER_H
#define SIGNALAMPLIFIER_H

#include "pelican/modules/AbstractModule.h"
#include "pelican/examples/SignalData.h"

using namespace pelican;

/*
 * A simple example to demonstrate how to write a pipeline module.
 */
class SignalAmplifier : public AbstractModule
{
    public:
        // Constructs the module.
        SignalAmplifier(const ConfigNode& config);

        // Runs the module.
        void run(const SignalData* input, SignalData* output);

    private:
        float gain;
};

PELICAN_DECLARE_MODULE(SignalAmplifier)

#endif // SIGNALAMPLIFIER_H
