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
        /// Constructs a test configuration reading the
        TestConfig(const QString fileName, QString package) : Config()
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
            read(QFileInfo(file).absolutePath());
        }

        ~TestConfig() {}
};

} // pelican
#endif /* TESTCONFIG_H_ */
