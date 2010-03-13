#ifndef TESTCONFIG_H_
#define TESTCONFIG_H_

/**
 * @file TestConfig.h
 */

#include "utility/Config.h"
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QString>
#include <iostream>
#include <QFileInfo>

namespace pelican {

/**
 * @class TestConfig
 *
 * @brief
 * Class for handling test configuration XML files.
 *
 * @details
 * Finds and locates test XML of the specified filename from any location in
 * the source tree.
 * Test XML files should be located in the test/data folder of the respective
 * package library.
 */

class TestConfig : public Config
{
    public:
        /// Constructs a test configuration object for the test configuration
        /// file of name /p fileName in the specified pelican pacakge folder
        /// /p package
        TestConfig(const QString& fileName, const QString& package) : Config() {
            read(findTestFile(fileName, package));
            _argv = NULL;
        }

        /// Constructs an empty test configuration object.
        TestConfig() : Config() {
            _argv = NULL;
        }

        /// Destroys the test configuration.
        ~TestConfig() {
            if (_argv) free(_argv);
        }

    public:
        /// Returns command line options for a given XML filename and package.
        char** argv(const QString& fileName, const QString& package)
        {
            if (_argv) free(_argv);
            QString path = findTestFile(fileName, package);
            QStringList args = QStringList() << "pelican" << "--config=" + path;
            int argc = args.count();
            _argv = (char**) malloc(argc * sizeof(char*));
            for (int i = 0; i < argc; i++) {
                _argv[i] = (char*) malloc(args[i].size() * sizeof(char) + 1);
                strcpy(_argv[i], args[i].toLatin1().data());
            }
            return _argv;
        }

    public:
        /// Returns the file path to a test file /p fileName for the specified
        /// package library /p package.
        static QString findTestFile(const QString& fileName, const QString& package)
        {
            QStringList searchPaths;
            searchPaths << "data";
            searchPaths << "test/data";
            searchPaths << package + "/test/data";
            searchPaths << "../" + package + "/test/data";
            searchPaths << "../../" + package + "/test/data";
            searchPaths << "../../../" + package + "/test/data";
            QDir::setSearchPaths("testData", searchPaths);
            QString name = "testData:" + fileName;
            QFile file(name);
            if (!file.exists()) {
                throw QString("TestConfig: Configuration file %1 not found.").arg(fileName);
            }
            return QFileInfo(file).absoluteFilePath();
        }

    private:
        char** _argv;
};

} // pelican
#endif /* TESTCONFIG_H_ */
