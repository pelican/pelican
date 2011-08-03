#ifndef TESTFILE_H
#define TESTFILE_H
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QList>


/**
 * @file TestFile.h
 */

namespace pelican {
namespace test {

/**
 * @class TestFile
 *
 * @brief
 *    Generates a unique filename for testing purposes
 *    Will also clean up the file on exit
 * @details
 *
 */
class TestFile
{
    public:
        /// TestFile constructor.
        TestFile(bool deleteFile = true );

        /// TestFile destructor.
        ~TestFile();

        QString filename();

    private:
        bool _cleanup;
        QMutex _mutex;
        QString _filename;
        static QList<QString> _reserved; // keep a track of reserved filenames
        static long _id;  // unique ids across all TestFiles
};

} // namespace test
} // namespace pelican

#endif // TESTFILE_H
