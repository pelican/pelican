#ifndef WATCHEDDIR_H
#define WATCHEDDIR_H


#include <QtCore/QFile>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QList>
class QByteArray;

/**
 * @file WatchedDir.h
 */

namespace pelican {

/**
 * @class WatchedDir
 *  
 * @brief
 *    A directory watcher that implemebnts the QIODevice interface
 * @details
 * 
 */

class WatchedDir : public QFile
{
    Q_OBJECT

    public:
        WatchedDir( const QString& name, QObject* parent = 0 );
        ~WatchedDir();
        virtual void setFileName( const QString& name );
        //virtual bool isSequential () const { return true; }
        QByteArray readFile();
        qint64 readFile( char* data, qint64 len );

    protected:
        void _openNext();

    private slots:
        void _dirChanged(const QString&);
        void _fileChanged(const QString&);

    private:
         QFileSystemWatcher _watcher;
         QList<QString> _streamFiles; // list of changed/new files to be read
         QStringList _files; // list of files at the last dir change
};

} // namespace pelican
#endif // WATCHEDDIR_H 
