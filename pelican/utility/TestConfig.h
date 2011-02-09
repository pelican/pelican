#ifndef TEST_CONFIG_H_
#define TEST_CONFIG_H_

/**
 * @file TestConfig.h
 */

#include "pelican/utility/Config.h"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QFileInfo>

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace pelican {
namespace test {

/**
 * @ingroup t_utility
 *
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
        /// file of name \p fileName in the specified Pelican package folder
        /// \p package.
        TestConfig(const QString& fileName, const QString& package) : Config() {
            setDocument(read(findTestFile(fileName, package)));
            _argc = 0;
            _argv = NULL;
        }

        /// Constructs an empty test configuration object.
        TestConfig() : Config() {
            _argc = 0;
            _argv = NULL;
        }

        /// Destroys the test configuration.
        ~TestConfig() {
            _freeArgv();
        }

    public:
        /// Returns command line options for a given XML filename and package.
        char** argv(const QString& fileName, const QString& package)
        {
            _freeArgv();
            QString path = findTestFile(fileName, package);
            QStringList args = QStringList() << "pelican" << "--config=" + path;
            _argc = args.count();
            _argv = (char**) malloc(_argc * sizeof(char*));
            for (int i = 0; i < _argc; i++) {
                _argv[i] = (char*) malloc(args[i].size() * sizeof(char) + 1);
                strcpy(_argv[i], args[i].toLatin1().data());
            }
            return _argv;
        }

        void saveTestConfig(const QString& fileName, const QString& package)
        {
            QString path = findTestFile(".", package);
            save(path + "/" +fileName);
        }

    public:
        /// Returns the file path to a test file \p fileName for the specified
        /// package library \p package.
        static QString findTestFile(const QString& fileName, const QString& package)
        {
#if QT_VERSION >= 0x040300
            QStringList searchPaths;
            searchPaths << "../../../" + package + "/   test/data";
            searchPaths << "../../" + package + "/test/data";
            searchPaths << "../" + package + "/test/data";
            searchPaths << package + "/test/data";
            searchPaths << "test/data";
            searchPaths << "data";
            QDir::setSearchPaths("testData", searchPaths);
            QString name = "testData:" + fileName;
#else
            QString name = fileName;
#endif
            QFile file(name);
            if (!file.exists()) {
                throw QString("TestConfig: Configuration file %1 not found.").arg(fileName);
            }
            return QFileInfo(file).filePath(); // absoluteFilePath() better ?
        }

    private:
        /// Frees the memory used by argv.
        void _freeArgv() {
            for (int i = 0; i < _argc; i++) {
                free(_argv[i]);
                _argv[i] = NULL;
            }
            free(_argv);
            _argv = NULL;
            _argc = 0;
        }

    private:
        int _argc;
        char** _argv;
};

} // test
} // pelican
#endif // TEST_CONFIG_H_
