#include "pelican/core/PipelineApplication.h"
#include "tutorial/SignalProcessingPipeline.h"
#include <QtCore/QCoreApplication>

// Include any headers that are referenced by name.
#include "tutorial/OutputStreamExample.h"
#include "tutorial/SignalDataAdapter.h"

int main(int argc, char* argv[])
{
    // Create a QCoreApplication.
    QCoreApplication app(argc, argv);

    try {
        // Create a PipelineApplication.
        PipelineApplication pApp(argc, argv);

        // Register the pipelines that can run.
        pApp.registerPipeline(new SignalProcessingPipeline);

        // Set the data client.
        pApp.setDataClient("PelicanServerClient");

        // Start the pipeline driver.
        pApp.start();
    }

    // Catch any error messages from Pelican.
    catch (const QString& err) {
        std::cerr << "Error: " << err.toStdString() << std::endl;
    }

    return 0;
}
