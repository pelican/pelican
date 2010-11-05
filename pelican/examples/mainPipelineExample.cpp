#include "pelican/core/PipelineApplication.h"
#include "pelican/examples/SignalProcessingPipeline.h"
#include <QtCore/QCoreApplication>

int main(int argc, char* argv[])
{
    // Create a QCoreApplication.
    QCoreApplication app(argc, argv);

    // Create a PipelineApplication.
    PipelineApplication pApp(argc, argv);

    // Register the pipelines that can run.
    pApp.registerPipeline(new SignalProcessingPipeline);

    // Set the data client.
    pApp.setDataClient("PelicanServerClient");

    // Start the pipeline driver.
    pApp.start();

    return 0;
}
