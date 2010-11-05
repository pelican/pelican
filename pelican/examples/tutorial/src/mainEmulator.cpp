#include "pelican/emulator/EmulatorDriver.h"
#include "pelican/utility/ConfigNode.h"
#include "tutorial/SignalEmulator.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

using namespace pelican;

int main(int argc, char* argv[])
{
    // Create a QCoreApplication.
    QCoreApplication app(argc, argv);

    // Create the emulator's XML configuration node from a string.
    ConfigNode xmlNode(
            "<SignalEmulator>"
            "    <packet samples=\"256\" interval=\"2560\" />"
            "    <signal period=\"20\" />"
            "    <connection host=\"127.0.0.1\" port=\"2001\" />"
            "</SignalEmulator>"
    );

    // Create the emulator and the emulator driver.
    EmulatorDriver driver(new SignalEmulator(xmlNode));
    return app.exec();
}
