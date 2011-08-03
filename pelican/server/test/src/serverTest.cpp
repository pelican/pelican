#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <QtCore/QCoreApplication>

int main(int argc, char** argv)
{
    // Get the top level suite from the registry
    CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

    // Adds the test to the list of test to run
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( suite );

    // Change the default outputter to a compiler error format outputter
    runner.setOutputter( new CppUnit::CompilerOutputter(
                &runner.result(),
                std::cerr ) );

    // Set up a QCore object to ensure data management is OK
    QCoreApplication* app = new QCoreApplication(argc, argv);

    // Run the tests.
    bool wasSucessful = runner.run();

    delete app;

    // Return error code 1 if the one of test failed.

    return wasSucessful ? 0 : 1;
}
