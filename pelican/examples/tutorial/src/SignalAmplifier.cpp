#include "tutorial/SignalAmplifier.h"
#include "tutorial/SignalData.h"
#include "pelican/utility/Config.h"

// Construct the example module.
SignalAmplifier::SignalAmplifier(const ConfigNode& config)
    : AbstractModule(config)
{
    // Set amplifier gain from the XML configuration.
    gain = config.getOption("gain", "value").toDouble();
}

// Runs the module.
void SignalAmplifier::run(const SignalData* input, SignalData* output)
{
    // Ensure the output storage data is big enough.
    unsigned nPts = input->size();
    if (output->size() != nPts)
        output->resize(nPts);

    // Get pointers to the memory to use from the data blobs.
    const float* in = input->ptr();
    float* out = output->ptr();

    // Perform the operation.
    for (unsigned i = 0; i < nPts; ++i) {
        out[i] = gain * in[i];
    }
}
