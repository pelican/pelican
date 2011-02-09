#ifndef WATCHEDFILE_H
#define WATCHEDFILE_H

/**
 * @file WatchedFile.h
 */

#include <QtCore/QFile>
#include <QtCore/QFileSystemWatcher>

namespace pelican {

/**
 * @ingroup c_utility
 *
 * @class WatchedFile
 *
 * @brief
 * A QFile that emits the readyRead() signal whenever it is updated
 *
 * @details
 */

class WatchedFile : public QFile
{
    Q_OBJECT

    public:
        /// WatchedFile constructor.
        WatchedFile( const QString& name, QObject* parent = 0 );
        WatchedFile( QObject* parent = 0 );

        /// WatchedFile destructor.
        ~WatchedFile();

        /// overriden setFileName operator
        void setFileName ( const QString & name );

    protected:
        void _init();

    private slots:
        void _fileChanged(const QString&);

    private:
         QFileSystemWatcher _watcher;
};

} // namespace pelican
#endif // WATCHEDFILE_H
