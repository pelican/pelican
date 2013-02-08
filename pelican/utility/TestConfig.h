/*
 * Copyright (c) 2013, The University of Oxford
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the University of Oxford nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TEST_CONFIG_H_
#define TEST_CONFIG_H_

/**
 * @file TestConfig.h
 */

#include "utility/Config.h"

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
